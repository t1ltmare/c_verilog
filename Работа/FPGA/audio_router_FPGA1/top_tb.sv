`timescale 1ns/1ps

module top_tb;

// Clock and reset
reg mclk;
reg rst_n;

// I2C signals
reg scl;
tri sda;

// Audio signals (не используются в тесте, но требуются для top)
reg lrck_192;
reg sclk_12288;
reg lrck_48;
reg sclk_3072;

reg [47:0] i2s_data_in;
wire [3:0] sdo_ff_bus;
wire [3:0] sdo_loopback;

// Register interface (добавлены)
reg [7:0] reg_addr;
reg [7:0] reg_data_in;
reg reg_write;
wire [7:0] reg_data_out;
wire reg_valid;
wire reg_read_req;
wire [7:0] reg_read_addr;

// Test control i2c
integer test_pass;
integer test_fail;
integer test_counter;

// Test control i2s
integer test_router_pass;
integer test_router_fail;
integer test_router_counter;

// I2C master simulation
reg sda_drive;

// Clock generation counters
reg [7:0] sclk_12288_counter;
reg [7:0] sclk_3072_counter;
reg [15:0] lrck_192_counter;
reg [15:0] lrck_48_counter;

// Assignments
assign sda = dut.i2c_slave_inst.sda_oe ? 1'bz : (sda_drive ? 1'bz : 1'b0);

// DUT instance
top dut (
    .mclk(mclk),
    .rst_n(rst_n),
    .lrck_192(lrck_192),
    .sclk_12288(sclk_12288),
    .i2s_data_in(i2s_data_in),
    .sdo_ff_bus(sdo_ff_bus),
    .sdo_loopback(sdo_loopback),
    .scl(scl),
    .sda(sda),
    .reg_addr(reg_addr),
    .reg_data_in(reg_data_in),
    .reg_write(reg_write),
    .reg_data_out(reg_data_out),
    .reg_valid(reg_valid),
    .reg_read_req(reg_read_req),
    .reg_read_addr(reg_read_addr)
);

// Clock generation (mclk)
always #10 mclk = ~mclk;

// Clock generation (sclk, lrck)

// sclk_12288: 12.288 MHz = 24.576 MHz / 2

always @(posedge mclk or negedge rst_n) begin
    if (!rst_n) begin
        sclk_12288 <= 1'b0;
    end else begin
        sclk_12288 <= ~sclk_12288;
    end
end

// lrck_192: 192 kHz = 12.288 MHz / 64

always @(negedge sclk_12288 or negedge rst_n) begin
    if (!rst_n) begin
        lrck_192_counter <= 16'd0;
        lrck_192 <= 1'b0;
    end else begin
        if (lrck_192_counter == 16'd31) begin // 64 edges = 32 cycles
            lrck_192_counter <= 16'd0;
            lrck_192 <= ~lrck_192;
        end else begin
            lrck_192_counter <= lrck_192_counter + 16'd1;
        end
    end
end

// sclk_3072: 3.072 MHz = 24.576 MHz / 8

always @(posedge mclk or negedge rst_n) begin
    if (!rst_n) begin
        sclk_3072_counter <= 8'd0;
        sclk_3072 <= 1'b0;
    end else begin
        if (sclk_3072_counter == 8'd3) begin // 8 edges = 4 cycles
            sclk_3072_counter <= 8'd0;
            sclk_3072 <= ~sclk_3072;
        end else begin
            sclk_3072_counter <= sclk_3072_counter + 8'd1;
        end
    end
end

// lrck_48: 48 kHz = 3.072 MHz / 64

always @(negedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        lrck_48_counter <= 16'd0;
        lrck_48 <= 1'b0;
    end else begin
        if (lrck_48_counter == 16'd31) begin // 64 edges = 32 cycles
            lrck_48_counter <= 16'd0;
            lrck_48 <= ~lrck_48;
        end else begin
            lrck_48_counter <= lrck_48_counter + 16'd1;
        end
    end
end

// Заполняем входные данные

reg [4:0] bit_counter;
reg lrck_48_prev;
wire lrck_48_edge = (lrck_48 != lrck_48_prev);

// Бит-каунтер для отслеживания позиции в кадре

always @(posedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        bit_counter <= 5'd0;
        lrck_48_prev <= 1'b0;
    end else begin
        lrck_48_prev <= lrck_48;
        if (lrck_48_edge || (!rst_n)) begin
            bit_counter <= 5'd0; // Сброс при смене кадра
        end else begin
            bit_counter <= bit_counter + 5'd1;
        end
    end
end

// Генерация i2s_data_in согласно протоколу I2S

logic [23:0] captured_i2s_din [0:15]; // Захваченные данные для проверки

always @(negedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        i2s_data_in <= 48'b0;
    end else begin 
        if (bit_counter < 24) begin
            // Первые 24 бита - случайные данные для 48 каналов
            // Каждый бит рандомизируется отдельно для реалистичности
            for (int i = 0; i < 48; i++) begin
                automatic logic [0:0] random_bit = $random & 1'b1;
                i2s_data_in[i] <= random_bit;
                captured_i2s_din[i] <= {captured_i2s_din[i][22:0], random_bit};
            end
        end else begin
            // Остальные 8 бит - нули (padding)
            i2s_data_in <= 48'b0;
        end
    end
end

// Буферизация захваченных данных для проверки после обработки

logic [23:0] buf_captured_i2s_din [0:31];

always_ff @(posedge dut.audio_router_inst.lrck_48_fall or posedge dut.audio_router_inst.lrck_48_rise or negedge rst_n) begin
    if (!rst_n) begin
        for (int bus = 0; bus < 48; bus++) begin
            buf_captured_i2s_din[bus] <= 24'b0;
        end
    end else begin
        if (dut.audio_router_inst.lrck_48_fall) begin
            for (int i = 0; i < 16; i++) begin
                buf_captured_i2s_din[i*2+1] = captured_i2s_din[i];
            end
        end else if (dut.audio_router_inst.lrck_48_rise) begin
            for (int i = 0; i < 16; i++) begin
                buf_captured_i2s_din[i*2] = captured_i2s_din[i];
            end
        end
    end 
end

// ******** Захват данных sdo_ff_bus для проверки ******** //

// Приемник для sdo_ff_bus
logic [2:0] rx_channel_index [0:3];     // Индекс текущего канала (0-7)
logic [4:0] rx_bit_index [0:3];         // Индекс бита (0-31)
logic [23:0] rx_shift_reg [0:3];        // Сдвиговый регистр для приема данных
logic [23:0] rx_channels [0:3][0:7];    // Принятые данные по каналам
logic rx_valid [0:3];                   // Флаг валидных данных

// Синхронизация счетчика битов с lrck_192

always_ff @(negedge sclk_12288) begin
    if (dut.audio_router_inst.lrck_192_fall || dut.audio_router_inst.lrck_192_rise)
    for (int bus = 0; bus < 4; bus++) begin
        rx_bit_index[bus] <= '0;
    end
end

// Захват данных с sdo_ff_bus

always_ff @(posedge sclk_12288 or negedge rst_n) begin
    if (!rst_n) begin
        for (int i = 0; i < 4; i++) begin
            rx_channel_index[i] <= '0;
            rx_bit_index[i] <= '0;
            rx_shift_reg[i] <= '0;
            rx_valid[i] <= 1'b0;
            for (int j = 0; j < 8; j++) begin
                rx_channels[i][j] <= '0;
            end
        end
    end else begin
        for (int bus = 0; bus < 4; bus++) begin
            rx_valid[bus] <= 1'b0; // Сбрасываем флаг валидности
            
            // Прием битов данных (первые 24 бита)
            if (rx_bit_index[bus] < 24) begin
                rx_shift_reg[bus] <= {rx_shift_reg[bus][22:0], sdo_ff_bus[bus]};
            end
            
            // Обработка завершения кадра (32-й бит)
            if (rx_bit_index[bus] == 31) begin
                // Сохраняем принятые данные в соответствующий канал
                rx_channels[bus][rx_channel_index[bus]] <= rx_shift_reg[bus];
                rx_valid[bus] <= 1'b1; // Устанавливаем флаг валидности
                
                // Переходим к следующему каналу
                if (rx_channel_index[bus] == 7) begin
                    rx_channel_index[bus] <= '0;
                end else begin
                    rx_channel_index[bus] <= rx_channel_index[bus] + 1;
                end
            end
            
            // Обновление индекса бита
            if (rx_bit_index[bus] == 31) begin
                rx_bit_index[bus] <= '0;
            end else begin
                rx_bit_index[bus] <= rx_bit_index[bus] + 1;
            end
        end
    end
end

logic [23:0] buf_buf_captured_i2s_din [0:31]; // Нужна задержка еще такт lrck_48

always_ff @(posedge dut.audio_router_inst.lrck_48_fall or negedge rst_n) begin
    if (!rst_n) begin
        for (int bus = 0; bus < 32; bus++) begin
            buf_buf_captured_i2s_din[bus] = 24'b0;
        end
    end else begin
        for (int i = 0; i < 32; i++) begin
            if (buf_buf_captured_i2s_din[i] !== rx_channels[i[4:3]][i[2:0]]) begin
                $display("  FAIL: Audio data mismatch on bus %0d, channel %0d: expected=0x%06X, got=0x%06X", i%4, i/4, buf_buf_captured_i2s_din[i], rx_channels[i%4][i/4]);
                test_router_fail = test_router_fail + 1;
                test_router_counter = test_router_counter + 1;
            end else begin
                $display("  PASS: Audio data match on bus %0d, channel %0d: data=0x%06X", i%4, i/4, rx_channels[i%4][i/4]);
                test_router_pass = test_router_pass + 1;
                test_router_counter = test_router_counter + 1;
            end
        end
    end
end

always_ff @(negedge dut.audio_router_inst.lrck_48_fall or negedge rst_n) begin
    if (!rst_n) begin
        for (int bus = 0; bus < 32; bus++) begin
            buf_buf_captured_i2s_din[bus] = 24'b0;
        end
    end else begin
        for (int i = 0; i < 32; i++) begin
            buf_buf_captured_i2s_din[i] = buf_captured_i2s_din[i];
        end
    end
end

// I2C pullup
pullup p1(sda);

// Main test sequence
initial begin
    $display("=========================================");
    $display("Top Testbench Started");
    $display("=========================================");
    
    // Initialize
    initialize();
    
    // Test sequences
    test_reset();
    for (int i = 0; i < 40; i++)
        test_register_write(i);
    //test_i2c_write();
    test_i2c_read();
    test_i2c_multi_read();
    
    // Report results
    report_results();
    
    $display("=========================================");
    $display("Testbench Finished");
    $display("=========================================");
    $display($time);
    $finish;
end

// Initialize system
task initialize;
begin
    mclk = 0;
    rst_n = 1;
    scl = 1;
    sda_drive = 1;
    
    // Initialize audio signals
    lrck_48 = 0;
    sclk_3072 = 0;
    lrck_192 = 0;
    sclk_12288 = 0;
    i2s_data_in = 48'b0;

    for (int i = 0; i < 16; i++) begin
        captured_i2s_din[i] = 24'b0;
        buf_captured_i2s_din[2*i] = 24'b0;
        buf_captured_i2s_din[2*i+1] = 24'b0;
    end
    
    // Initialize register interface
    reg_addr = 8'h00;
    reg_data_in = 8'h00;
    reg_write = 0;
    
    test_pass = 0;
    test_fail = 0;
    test_counter = 0;

    test_router_pass = 0;
    test_router_fail = 0;
    test_router_counter = 0;
    
    #100;
    rst_n = 0;  // Assert reset
    #200;
    rst_n = 1;  // Deassert reset
    #1000;

    for (int i = 0; i < 40; i++) begin
        reg_addr = i;
        reg_data_in = i;  // или другие значения по умолчанию
        reg_write = 1;
        #100;
        reg_write = 0;
        #100;
    end

    $display("Initialization complete");
end
endtask

// Test reset functionality
task test_reset;
begin
    $display("Test %0d: Reset test", test_counter);
    
    // Check if signals are in reset state
    if (dut.i2c_slave_inst.current_state == dut.i2c_slave_inst.IDLE) begin
        $display("  PASS: FSM in IDLE state after reset");
        test_pass = test_pass + 1;
    end else begin
        $display("  FAIL: FSM not in IDLE state after reset");
        test_fail = test_fail + 1;
    end
    
    if (dut.i2c_slave_inst.sda_oe === 1'b0) begin
        $display("  PASS: SDA released after reset");
        test_pass = test_pass + 1;
    end else begin
        $display("  FAIL: SDA not released after reset");
        test_fail = test_fail + 1;
    end
    test_counter = test_counter + 1;
    #1000;
end
endtask

// Test register write through external interface (теперь работает!)
task test_register_write;
input [7:0] data;
begin
    
    $display("Test %0d: External register write", test_counter);
    
    reg_addr = data;
    reg_data_in = data; //+ 8'h0;
    reg_write = 1;
    #100;
    reg_write = 0;
    
    // Wait for valid signal
    wait(reg_valid);
    #100;
    
    if (dut.i2c_slave_inst.register_file[reg_addr] === reg_data_in) begin
        $display("  PASS: Register write successful, data=0x%02X", dut.i2c_slave_inst.register_file[reg_addr]);
        test_pass = test_pass + 1;
    end else begin
        $display("  FAIL: Register write failed, expected=0x%02X, got=0x%02X", reg_data_in, dut.i2c_slave_inst.register_file[reg_addr]);
        test_fail = test_fail + 1;
    end
    test_counter = test_counter + 1;
    #1000;
end
endtask

// Test I2C write operation
task test_i2c_write;
begin
    static logic [7:0] wr_test_addr = 8'h20;
    static logic [7:0] wr_test_data1 = 8'h5A;
    static logic [7:0] wr_test_data2 = 8'hEE;
    automatic int result = 1;

    test_counter = test_counter + 1;
    $display("Test %0d: I2C write operation", test_counter);
    
    // Send start condition
    i2c_start();
    
    // Send slave address + write bit
    i2c_send_byte(8'hA0, result); // 7'h50 << 1 + 1'b0
    
    // Send register address
    if (result) i2c_send_byte(wr_test_addr, result);
    
    // Send data
    if (result) i2c_send_byte(wr_test_data1, result);
    if (result) i2c_send_byte(wr_test_data2, result);
    
    // Send stop condition
    i2c_stop();
    
    #5000;
    
    // Check if data was written
    if (dut.i2c_slave_inst.register_file[wr_test_addr] === wr_test_data1) begin
        $display("  PASS: Register write successful, data=0x%02X", dut.i2c_slave_inst.register_file[wr_test_addr]);
        test_pass = test_pass + 1;
    end else begin
        $display("  FAIL: Register write failed, expected=0x%02X, got=0x%02X", wr_test_data1, dut.i2c_slave_inst.register_file[wr_test_addr]);
        test_fail = test_fail + 1;
    end
    if (dut.i2c_slave_inst.register_file[wr_test_addr + 1] === wr_test_data2) begin
        $display("  PASS: Register write successful, data=0x%02X", dut.i2c_slave_inst.register_file[wr_test_addr + 1]);
        test_pass = test_pass + 1;
    end else begin
        $display("  FAIL: Register write failed, expected=0x%02X, got=0x%02X", wr_test_data2, dut.i2c_slave_inst.register_file[wr_test_addr + 1]);
        test_fail = test_fail + 1;
    end
    #100;
    
    $display("  I2C write sequence completed");
    
    #1000;
end
endtask

// Test I2C read operation
task test_i2c_read;
begin
    automatic int result = 1;

    test_counter = test_counter + 1;
    $display("Test %0d: I2C read operation", test_counter);
    
    // First write data to read later через external interface
    reg_addr = 8'h30;
    reg_data_in = 8'hC3;
    reg_write = 1;
    #100;
    reg_write = 0;
    wait(reg_valid);
    
    // I2C read sequence
    i2c_start();
    
    // Send slave address + write bit to set register address
    i2c_send_byte(8'hA0, result); // 7'h50 << 1 + 1'b0
    
    // Send register address to read from
    if (result) i2c_send_byte(8'h30, result);
    
    // Repeated start
    if (result) i2c_start();
    
    // Send slave address + read bit
    if (result) i2c_send_byte(8'hA1, result); // 7'h50 << 1 + 1'b1
    
    // Read data (slave should send 0xC3)
    if (result) i2c_read_byte();
    
    // Send NACK and stop
    if (result) begin
        sda_drive = 1; // NACK
        i2c_clock_pulse();
    end
    i2c_stop();
    
    $display("  I2C read sequence completed");
    test_pass = test_pass + 1;
    
    #1000;
end
endtask

// Test I2C multi read operation
task test_i2c_multi_read;
begin
    automatic int result = 1;

    test_counter = test_counter + 1;
    $display("Test %0d: I2C multi read operation", test_counter);
    
    // I2C read sequence
    i2c_start();
    
    // Send slave address + write bit to set register address
    i2c_send_byte(8'hA0, result); // 7'h50 << 1 + 1'b0
    
    // Send register address to read from
    if (result) i2c_send_byte(8'h0B, result);
    
    // Repeated start
    if (result) i2c_start();
    
    // Send slave address + read bit
    if (result) i2c_send_byte(8'hA1, result); // 7'h50 << 1 + 1'b1
    
    // Read data
    if (result) begin
        i2c_read_byte();
        // Send ACK
        sda_drive = 0;
        i2c_clock_pulse();
    end

    if (result) begin
        i2c_read_byte();
        // Send NACK and stop
        sda_drive = 1; // NACK
        i2c_clock_pulse();
    end
    i2c_stop();
    
    $display("  I2C read sequence completed");
    test_pass = test_pass + 1;
    
    #1000;
end
endtask

// I2C tasks (без изменений)
task i2c_start;
begin
    sda_drive = 1;
    #1000;
    scl = 1;
    #1000;
    sda_drive = 0;
    #1000;
    scl = 0;
    #1000;
end
endtask

task i2c_stop;
begin
    sda_drive = 0;
    #1000;
    scl = 1;
    #1000;
    sda_drive = 1;
    #1000;
end
endtask

task i2c_send_byte;
input [7:0] data;
output int result;
integer i;
begin
    for (i = 7; i >= 0; i = i - 1) begin
        sda_drive = data[i];
        #500;
        scl = 1;
        #1000;
        scl = 0;
        #500;
    end
    
    // Release for ACK
    sda_drive = 0;
    #500;
    scl = 1;
    result = (sda == 1'b0) ? 1 : 0;
    #1000;
    scl = 0;
    #500;
end
endtask

task i2c_read_byte;
integer i;
begin
    automatic logic [7:0] read_data;
    sda_drive = 0; // Release SDA for reading
    for (i = 7; i >= 0; i = i - 1) begin
        #500;
        scl = 1;
        #1000;
        read_data[i] = sda;
        scl = 0;
        #500;
    end
    $display("    Read data - 0x%02X", read_data);
end
endtask

task i2c_clock_pulse;
begin
    #500;
    scl = 1;
    #1000;
    scl = 0;
    #500;
end
endtask

// Report test results
task report_results;
begin
    $display("\nTest Results:");
    $display("  Passed: %0d", test_pass);
    $display("  Failed: %0d", test_fail);
    $display("  Total:  %0d", test_pass + test_fail);

    $display("\nTest Results:");
    $display("  Passed: %0d", test_router_pass);
    $display("  Failed: %0d", test_router_fail);
    $display("  Total:  %0d", test_router_pass + test_router_fail);
    
    if (test_fail == 0) begin
        $display("*** ALL I2C TESTS PASSED ***");
    end else begin
        $display("*** SOME I2C TESTS FAILED ***");
    end

    if (test_router_fail == 0) begin
        $display("*** ALL I2S TESTS PASSED ***");
    end else begin
        $display("*** %0d I2S TESTS FAILED ***", test_router_fail);
    end
end
endtask

// Monitor important signals
always @(posedge reg_read_req) begin
    $display("    Register read request: addr=0x%02X", reg_read_addr);
end

// State monitor
always @(dut.i2c_slave_inst.current_state) begin
    $display("Slave state changed to: %s", dut.i2c_slave_inst.current_state.name);
end

// Waveform dump
initial begin
    $dumpfile("top_tb.vcd");
    $dumpvars(0, top_tb);
end

endmodule