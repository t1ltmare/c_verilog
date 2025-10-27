// MCLK = 24,576 MHz
// SCLK = MCLK / 8 = 3,072 MHz
// LRCK = MCLK / 512 = 0,048 MHz

module i2s_generator (
    input clk,
    output reg sclk,
    output reg lrck,
    output reg i2s1_data,           // I2S1 Data
    output reg i2s2_data            // I2S2 Data
);

    localparam [15:0] RAND1 = 16'b1100110011100011; // Данные для левого канала
    localparam [15:0] RAND2 = 16'b1100111000110011; // Данные для правого канала
    localparam [15:0] RAND3 = 16'b1101010001110011;
    localparam [15:0] RAND4 = 16'b1110101000111001;
    localparam [15:0] RAND5 = 16'b1000011110011001;
    localparam [15:0] RAND6 = 16'b1000001111100011;

    reg [4:0] bit_cnt;              // Counter for bit transmission (0-31)
    reg [0:0] lrck_d;                     // Delayed LRCK for edges detect
    reg [1:0] cnt_sclk;
    reg [7:0] cnt_lrck;
    reg [1:0] cnt_pack;
    
    initial begin
        cnt_sclk = 0;
        cnt_lrck = 0;
        cnt_pack = 0;
        bit_cnt = 0;
        sclk = 0;
        lrck = 0;
        lrck_d = 0;
    end

    always @(posedge clk) begin
        if (cnt_sclk == 3) begin                // счетчик sclk
            cnt_sclk <= 0;
            sclk <= ~sclk;
        end else if (cnt_sclk < 3) cnt_sclk <= cnt_sclk + 2'b001;

        if (cnt_lrck == 255) begin              // счетчик lrck
            cnt_lrck <= 0; 
            lrck <= ~lrck;
        end else if (cnt_lrck < 255) cnt_lrck <= cnt_lrck + 8'd1;
    end

    always @(negedge sclk) begin
        if ((!lrck) && lrck_d) begin
            bit_cnt <= 0;                           // Reset counter on LRCK falling edge
            if (cnt_pack == 2'b10) cnt_pack <= 2'b00;
            else cnt_pack <= cnt_pack + 2'b01;
        end else if (lrck && (!lrck_d)) begin
            bit_cnt <= 0;                           // Reset counter on LRCK rising edge
            if (cnt_pack == 2'b10) cnt_pack <= 2'b00;
            else cnt_pack <= cnt_pack + 2'b01;
        end
        else bit_cnt <= bit_cnt + 5'b00001;         // Else increment counter
        lrck_d <= lrck;
        if (bit_cnt < 16) begin                     // Random data 16 bit
            if (cnt_pack == 0) begin
                i2s1_data <= RAND1[15 - bit_cnt];
                i2s2_data <= RAND2[15 - bit_cnt];
            end else if (cnt_pack == 1) begin
                i2s1_data <= RAND3[15 - bit_cnt];
                i2s2_data <= RAND4[15 - bit_cnt];
            end else if (cnt_pack == 2) begin
                i2s1_data <= RAND5[15 - bit_cnt];
                i2s2_data <= RAND6[15 - bit_cnt];
            end
        end else if (bit_cnt < 32) begin        // Zero 16 bit
            i2s1_data <= 1'b0;
            i2s2_data <= 1'b0;                     
        end
    end
endmodule