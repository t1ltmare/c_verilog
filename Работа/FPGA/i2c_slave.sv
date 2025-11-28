module i2c_slave (
    // System signals
    input wire clk,
    input wire rst_n,
    
    // I2C physical interface
    input wire scl,
    inout wire sda,
    
    // Register interface
    output wire [7:0] reg_addr,
    input  wire [7:0] rd_data,
    output wire [7:0] wr_data,
    output wire       wr_enable
);

// Parameters
parameter SLAVE_ADDRESS = 7'h50;
parameter REGISTERS_COUNT = 256;

// Internal signals
reg sda_out;
reg sda_oe;
reg sda_sync;
reg scl_sync;
reg sda_falling;
reg sda_rising;
reg scl_falling;
reg scl_rising;

// Synchronizers for metastability
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        sda_sync <= 1'b1;
        scl_sync <= 1'b1;
    end else begin
        sda_sync <= sda;
        scl_sync <= scl;
    end
end

// Edge detection
reg sda_delayed, scl_delayed;
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        sda_delayed <= 1'b1;
        scl_delayed <= 1'b1;
        sda_falling <= 1'b0;
        sda_rising <= 1'b0;
        scl_falling <= 1'b0;
        scl_rising <= 1'b0;
    end else begin
        sda_delayed <= sda_sync;
        scl_delayed <= scl_sync;
        
        sda_falling <= sda_delayed && !sda_sync;
        sda_rising <= !sda_delayed && sda_sync;
        scl_falling <= scl_delayed && !scl_sync;
        scl_rising <= !scl_delayed && scl_sync;
    end
end

// FSM states
typedef enum logic [3:0] {
    IDLE,
    START_COND,
    RECV_SLAVE_ADDR,
    RECV_REG_ADDR,
    RECV_WRITE_DATA,
    SEND_READ_DATA,
    SEND_ACK_SLAVE,
    SEND_ACK_REG,
    SEND_ACK_DATA,
    SEND_NACK,
    RECV_ACK,
    STOP_COND
} state_t;

state_t current_state, next_state;

// Shift register and counters
reg  [7:0] shift_reg;
wire [7:0] shifted_data = {shift_reg[6:0], sda_sync};
reg  [7:0] current_reg_addr;
wire [7:0] next_reg_addr = current_reg_addr + 1'b1;
reg  [2:0] bit_counter;
reg        address_matched;
reg        read_mode;

// Tri-state buffer for SDA
assign sda = sda_oe ? sda_out : 1'bz;

// FSM state register
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        current_state <= IDLE;
    else
        current_state <= next_state;
end

// FSM next state logic
always @(*) begin
    next_state = current_state;
    
    case (current_state)
        IDLE: begin
            if (sda_falling && scl_sync) 
                next_state = START_COND;
        end
        
        START_COND: begin
            if (scl_falling)
                next_state = RECV_SLAVE_ADDR;
        end
        
        RECV_SLAVE_ADDR: begin
            if (bit_counter == 3'd7 && scl_rising)
                    next_state = address_matched ? SEND_ACK_SLAVE : SEND_NACK;
        end
        
        SEND_ACK_SLAVE: begin
            if (scl_rising)
                next_state = read_mode ? SEND_READ_DATA : RECV_REG_ADDR;
        end
        
        SEND_NACK: begin
            if (scl_rising)
                next_state = IDLE;
        end

        RECV_REG_ADDR: begin
            if (bit_counter == 3'd7 && scl_rising)
                next_state = shifted_data < REGISTERS_COUNT ? SEND_ACK_REG : SEND_NACK;
        end
        
        SEND_ACK_REG: begin
            if (scl_rising)
                next_state = read_mode ? IDLE : RECV_WRITE_DATA;
        end
                
        RECV_WRITE_DATA: begin
            if (bit_counter == 3'd7 && scl_rising)
                next_state = SEND_ACK_DATA;
        end
        
        SEND_ACK_DATA: begin
            if (scl_rising)
                next_state = read_mode ? IDLE : RECV_WRITE_DATA;
        end
                
        SEND_READ_DATA: begin
            if (bit_counter == 3'd7 && scl_rising)
                next_state = RECV_ACK;
        end
        
        RECV_ACK: begin
            if (scl_rising) begin
                if (!sda_sync) // ACK received
                    next_state = read_mode ? SEND_READ_DATA : IDLE;
                else // NACK
                    next_state = IDLE;
            end
        end
        
        STOP_COND: begin
            if (sda_rising && scl_sync)
                next_state = IDLE;
        end
    endcase
    
    // Detect stop condition
    if (sda_rising && scl_sync && current_state != IDLE)
        next_state = STOP_COND;
    
    // Detect repeated start
    if (sda_falling && scl_sync && current_state != IDLE)
        next_state = START_COND;
end

// Bit counter
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        bit_counter <= 3'd0;
    else begin
        if (current_state == START_COND || current_state == STOP_COND)
            bit_counter <= 1'b0;
        else if (scl_rising) begin
            case (current_state)
                RECV_SLAVE_ADDR, RECV_REG_ADDR, RECV_WRITE_DATA, SEND_READ_DATA: begin
                    if (bit_counter == 3'd7)
                        bit_counter <= 3'd0;
                    else
                        bit_counter <= bit_counter + 1'b1;
                end
                default: begin
                    bit_counter <= 3'd0;
                end
            endcase
        end
    end
end

// Shift register for receiving data
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        shift_reg <= 8'h00;
        read_mode <= 1'b0;
        current_reg_addr <= 8'h00;
    end else begin
        if (scl_rising) begin
            case (current_state)
                RECV_SLAVE_ADDR: begin
                    shift_reg <= shifted_data;
                    if (bit_counter == 3'd7)
                        read_mode <= sda_sync; // R/W bit
                end
                
                RECV_REG_ADDR: begin
                    shift_reg <= shifted_data;
                    if (bit_counter == 3'd7)
                        current_reg_addr <= shifted_data;
                end
                
                RECV_WRITE_DATA: begin
                    shift_reg <= shifted_data;
                    if (bit_counter == 3'd7)
                        // Auto-increment register address with automatic range crossing
                        current_reg_addr <= next_reg_addr == REGISTERS_COUNT ? 8'h00 : next_reg_addr;
                end
                
                SEND_READ_DATA: begin
                    if (bit_counter == 3'd7)
                        // Auto-increment register address with automatic range crossing
                        current_reg_addr <= next_reg_addr == REGISTERS_COUNT ? 8'h00 : next_reg_addr;
                end
            endcase
        end
    end
end

// Slave address matching
always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        address_matched <= 1'b0;
    else
        if (scl_falling && current_state == RECV_SLAVE_ADDR && bit_counter == 3'd7)
            address_matched <= (shift_reg[6:0] == SLAVE_ADDRESS);
end

// External registers interface handling
assign wr_enable = (current_state == RECV_WRITE_DATA && bit_counter == 3'd7 && scl_rising);
assign wr_data = shifted_data;
assign reg_addr = current_reg_addr;

// SDA output control
always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        sda_out <= 1'b1;
        sda_oe <= 1'b0;
    end else if (scl_falling) begin
        case (current_state)
            IDLE: begin
                sda_out <= 1'b1;
                sda_oe <= 1'b0;
            end
            
            SEND_ACK_SLAVE: begin
                sda_out <= 1'b0; // Send ACK
                sda_oe <= 1'b1;
            end
            
            SEND_ACK_REG: begin
                sda_out <= 1'b0; // Send ACK
                sda_oe <= 1'b1;
            end
            
            SEND_ACK_DATA: begin
                sda_out <= 1'b0; // Send ACK
                sda_oe <= 1'b1;
            end
            
            SEND_NACK: begin
                sda_out <= 1'b1; // Send NACK
                sda_oe <= 1'b1;
            end
            
            SEND_READ_DATA: begin
                sda_out <= rd_data[7 - bit_counter];
                sda_oe <= 1'b1;
            end
            
            default: begin
                sda_out <= 1'b1;
                sda_oe <= 1'b0;
            end
        endcase
    end
end

endmodule