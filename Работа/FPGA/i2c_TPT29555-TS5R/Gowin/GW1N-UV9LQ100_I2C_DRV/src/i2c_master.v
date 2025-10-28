module i2c_master
#(
	parameter OFFSET = 100
)
(
    input mclk,
    input  aresetn,
    //-------------
    output reg scl_reg,
    output reg sda_reg
);
       
reg [8:0] counter;
reg [6:0] clk_counter; 
reg [0:0] i2c_clk_reg;

localparam [0:6] ADDR_D4 = 7'b0100000;
localparam [0:6] ADDR_D5 = 7'b0100001;
localparam [0:6] ADDR_D7 = 7'b0100010;
localparam [0:6] ADDR_D8 = 7'b0100011;

reg [0:6] addr = ADDR_D8;   // Choose address

reg [0:7] command_config = 8'b00000110;
reg [0:7] config_port0 = 8'b11111111;   // P0_7, P0_6, P0_5, ...    (1 = input, 0 = output)
reg [0:7] config_port1 = 8'b00000000;   // P1_7, P1_6, P1_5, ...    (1 = input, 0 = output)

reg [0:7] command_output = 8'b00000010;
reg [0:7] output_port0 = 8'b11111111;   // P0_7, P0_6, P0_5, ...    (1 = H, 0 = L)
reg [0:7] output_port1 = 8'b11101111;   // P1_7, P1_6, P1_5, ...    (1 = H, 0 = L)

initial begin
    scl_reg <= 1;
    sda_reg <= 1;
end

always @(posedge mclk or negedge aresetn) begin
    if (!aresetn) begin
        clk_counter <= 0;
        i2c_clk_reg <= 0;
    end else begin
        if (clk_counter == 127) begin
            clk_counter <= 0;
        end else begin
            clk_counter <= clk_counter + 1;
        end
        i2c_clk_reg <= clk_counter[4];
    end
end

always @(posedge i2c_clk_reg, negedge aresetn) begin
    if (aresetn == 0) counter <= 0;
    else if (counter < 511) counter <= counter +1;
end

/*
// Паттерн для 21 такта (0-20)
localparam [0:20] SCL_PATTERN = 21'b001010101010101010100;

always @(*) begin
    if (counter >= OFFSET + 2 && counter <= OFFSET + 22) begin
        // Первый блок: относительная позиция 0-20
        scl_reg <= SCL_PATTERN[counter - OFFSET - 2];
    end else if (counter >= OFFSET + 23 && counter <= OFFSET + 43) begin
        // Второй блок: относительная позиция 0-20
        scl_reg <= SCL_PATTERN[counter - OFFSET - 23];
    end else begin
        scl_reg <= 1;
    end
end
*/

always @(posedge i2c_clk_reg) begin
    if (counter == OFFSET + 2) scl_reg <= 0;
    else if (counter == OFFSET + 3) scl_reg <= 0;
    else if (counter == OFFSET + 4) scl_reg <= 1; // Addr 6
    else if (counter == OFFSET + 5) scl_reg <= 0;
    else if (counter == OFFSET + 6) scl_reg <= 1; // Addr 5
    else if (counter == OFFSET + 7) scl_reg <= 0;
    else if (counter == OFFSET + 8) scl_reg <= 1; // Addr 4
    else if (counter == OFFSET + 9) scl_reg <= 0;
    else if (counter == OFFSET + 10) scl_reg <= 1; // Addr 3
    else if (counter == OFFSET + 11) scl_reg <= 0;
    else if (counter == OFFSET + 12) scl_reg <= 1; // Addr 2
    else if (counter == OFFSET + 13) scl_reg <= 0;
    else if (counter == OFFSET + 14) scl_reg <= 1; // Addr 1
    else if (counter == OFFSET + 15) scl_reg <= 0;
    else if (counter == OFFSET + 16) scl_reg <= 1; // Addr 0
    else if (counter == OFFSET + 17) scl_reg <= 0;
    else if (counter == OFFSET + 18) scl_reg <= 1; // WR
    else if (counter == OFFSET + 19) scl_reg <= 0;
    else if (counter == OFFSET + 20) scl_reg <= 1; // ACK
    else if (counter == OFFSET + 21) scl_reg <= 0;
    else if (counter == OFFSET + 22) scl_reg <= 0;
    
    else if (counter == OFFSET + 23) scl_reg <= 0;
    else if (counter == OFFSET + 24) scl_reg <= 0;
    else if (counter == OFFSET + 25) scl_reg <= 1; // Data 7
    else if (counter == OFFSET + 26) scl_reg <= 0;
    else if (counter == OFFSET + 27) scl_reg <= 1; // Data  6
    else if (counter == OFFSET + 28) scl_reg <= 0;
    else if (counter == OFFSET + 29) scl_reg <= 1; // Data  5
    else if (counter == OFFSET + 30) scl_reg <= 0;
    else if (counter == OFFSET + 31) scl_reg <= 1; // Data  4
    else if (counter == OFFSET + 32) scl_reg <= 0;
    else if (counter == OFFSET + 33) scl_reg <= 1; // Data  3
    else if (counter == OFFSET + 34) scl_reg <= 0;
    else if (counter == OFFSET + 35) scl_reg <= 1; // Data  2
    else if (counter == OFFSET + 36) scl_reg <= 0;
    else if (counter == OFFSET + 37) scl_reg <= 1; // Data  1
    else if (counter == OFFSET + 38) scl_reg <= 0;
    else if (counter == OFFSET + 39) scl_reg <= 1; // Data  0
    else if (counter == OFFSET + 40) scl_reg <= 0;
    else if (counter == OFFSET + 41) scl_reg <= 1; // ACK
    else if (counter == OFFSET + 42) scl_reg <= 0;
    else if (counter == OFFSET + 43) scl_reg <= 0;

    else if (counter == OFFSET + 44) scl_reg <= 0;
    else if (counter == OFFSET + 45) scl_reg <= 0;
    else if (counter == OFFSET + 46) scl_reg <= 1; // Data 7
    else if (counter == OFFSET + 47) scl_reg <= 0;
    else if (counter == OFFSET + 48) scl_reg <= 1; // Data  6
    else if (counter == OFFSET + 49) scl_reg <= 0;
    else if (counter == OFFSET + 50) scl_reg <= 1; // Data  5
    else if (counter == OFFSET + 51) scl_reg <= 0;
    else if (counter == OFFSET + 52) scl_reg <= 1; // Data  4
    else if (counter == OFFSET + 53) scl_reg <= 0;
    else if (counter == OFFSET + 54) scl_reg <= 1; // Data  3
    else if (counter == OFFSET + 55) scl_reg <= 0;
    else if (counter == OFFSET + 56) scl_reg <= 1; // Data  2
    else if (counter == OFFSET + 57) scl_reg <= 0;
    else if (counter == OFFSET + 58) scl_reg <= 1; // Data  1
    else if (counter == OFFSET + 59) scl_reg <= 0;
    else if (counter == OFFSET + 60) scl_reg <= 1; // Data  0
    else if (counter == OFFSET + 61) scl_reg <= 0;
    else if (counter == OFFSET + 62) scl_reg <= 1; // ACK
    else if (counter == OFFSET + 63) scl_reg <= 0;
    else if (counter == OFFSET + 64) scl_reg <= 0;

    else if (counter == OFFSET + 65) scl_reg <= 0;
    else if (counter == OFFSET + 66) scl_reg <= 0;
    else if (counter == OFFSET + 67) scl_reg <= 1; // Data 7
    else if (counter == OFFSET + 68) scl_reg <= 0;
    else if (counter == OFFSET + 69) scl_reg <= 1; // Data  6
    else if (counter == OFFSET + 70) scl_reg <= 0;
    else if (counter == OFFSET + 71) scl_reg <= 1; // Data  5
    else if (counter == OFFSET + 72) scl_reg <= 0;
    else if (counter == OFFSET + 73) scl_reg <= 1; // Data  4
    else if (counter == OFFSET + 74) scl_reg <= 0;
    else if (counter == OFFSET + 75) scl_reg <= 1; // Data  3
    else if (counter == OFFSET + 76) scl_reg <= 0;
    else if (counter == OFFSET + 77) scl_reg <= 1; // Data  2
    else if (counter == OFFSET + 78) scl_reg <= 0;
    else if (counter == OFFSET + 79) scl_reg <= 1; // Data  1
    else if (counter == OFFSET + 80) scl_reg <= 0;
    else if (counter == OFFSET + 81) scl_reg <= 1; // Data  0
    else if (counter == OFFSET + 82) scl_reg <= 0;
    else if (counter == OFFSET + 83) scl_reg <= 1; // ACK
    else if (counter == OFFSET + 84) scl_reg <= 0;
    else if (counter == OFFSET + 85) scl_reg <= 0;

    else if (counter == OFFSET + 95) scl_reg <= 0;
    else if (counter == OFFSET + 96) scl_reg <= 0;
    else if (counter == OFFSET + 97) scl_reg <= 1; // Addr 6
    else if (counter == OFFSET + 98) scl_reg <= 0;
    else if (counter == OFFSET + 99) scl_reg <= 1; // Addr 5
    else if (counter == OFFSET + 100) scl_reg <= 0;
    else if (counter == OFFSET + 101) scl_reg <= 1; // Addr 4
    else if (counter == OFFSET + 102) scl_reg <= 0;
    else if (counter == OFFSET + 103) scl_reg <= 1; // Addr 3
    else if (counter == OFFSET + 104) scl_reg <= 0;
    else if (counter == OFFSET + 105) scl_reg <= 1; // Addr 2
    else if (counter == OFFSET + 106) scl_reg <= 0;
    else if (counter == OFFSET + 107) scl_reg <= 1; // Addr 1
    else if (counter == OFFSET + 108) scl_reg <= 0;
    else if (counter == OFFSET + 109) scl_reg <= 1; // Addr 0
    else if (counter == OFFSET + 110) scl_reg <= 0;
    else if (counter == OFFSET + 111) scl_reg <= 1; // WR
    else if (counter == OFFSET + 112) scl_reg <= 0;
    else if (counter == OFFSET + 113) scl_reg <= 1; // ACK
    else if (counter == OFFSET + 114) scl_reg <= 0;
    else if (counter == OFFSET + 115) scl_reg <= 0;

    else if (counter == OFFSET + 116) scl_reg <= 0;
    else if (counter == OFFSET + 117) scl_reg <= 0;
    else if (counter == OFFSET + 118) scl_reg <= 1; // Data 7
    else if (counter == OFFSET + 119) scl_reg <= 0;
    else if (counter == OFFSET + 120) scl_reg <= 1; // Data  6
    else if (counter == OFFSET + 121) scl_reg <= 0;
    else if (counter == OFFSET + 122) scl_reg <= 1; // Data  5
    else if (counter == OFFSET + 123) scl_reg <= 0;
    else if (counter == OFFSET + 124) scl_reg <= 1; // Data  4
    else if (counter == OFFSET + 125) scl_reg <= 0;
    else if (counter == OFFSET + 126) scl_reg <= 1; // Data  3
    else if (counter == OFFSET + 127) scl_reg <= 0;
    else if (counter == OFFSET + 128) scl_reg <= 1; // Data  2
    else if (counter == OFFSET + 129) scl_reg <= 0;
    else if (counter == OFFSET + 130) scl_reg <= 1; // Data  1
    else if (counter == OFFSET + 131) scl_reg <= 0;
    else if (counter == OFFSET + 132) scl_reg <= 1; // Data  0
    else if (counter == OFFSET + 133) scl_reg <= 0;
    else if (counter == OFFSET + 134) scl_reg <= 1; // ACK
    else if (counter == OFFSET + 135) scl_reg <= 0;
    else if (counter == OFFSET + 136) scl_reg <= 0;

    else if (counter == OFFSET + 137) scl_reg <= 0;
    else if (counter == OFFSET + 138) scl_reg <= 0;
    else if (counter == OFFSET + 139) scl_reg <= 1; // Data 7
    else if (counter == OFFSET + 140) scl_reg <= 0;
    else if (counter == OFFSET + 141) scl_reg <= 1; // Data  6
    else if (counter == OFFSET + 142) scl_reg <= 0;
    else if (counter == OFFSET + 143) scl_reg <= 1; // Data  5
    else if (counter == OFFSET + 144) scl_reg <= 0;
    else if (counter == OFFSET + 145) scl_reg <= 1; // Data  4
    else if (counter == OFFSET + 146) scl_reg <= 0;
    else if (counter == OFFSET + 147) scl_reg <= 1; // Data  3
    else if (counter == OFFSET + 148) scl_reg <= 0;
    else if (counter == OFFSET + 149) scl_reg <= 1; // Data  2
    else if (counter == OFFSET + 150) scl_reg <= 0;
    else if (counter == OFFSET + 151) scl_reg <= 1; // Data  1
    else if (counter == OFFSET + 152) scl_reg <= 0;
    else if (counter == OFFSET + 153) scl_reg <= 1; // Data  0
    else if (counter == OFFSET + 154) scl_reg <= 0;
    else if (counter == OFFSET + 155) scl_reg <= 1; // ACK
    else if (counter == OFFSET + 156) scl_reg <= 0;
    else if (counter == OFFSET + 157) scl_reg <= 0;

    else if (counter == OFFSET + 158) scl_reg <= 0;
    else if (counter == OFFSET + 159) scl_reg <= 0;
    else if (counter == OFFSET + 160) scl_reg <= 1; // Data 7
    else if (counter == OFFSET + 161) scl_reg <= 0;
    else if (counter == OFFSET + 162) scl_reg <= 1; // Data  6
    else if (counter == OFFSET + 163) scl_reg <= 0;
    else if (counter == OFFSET + 164) scl_reg <= 1; // Data  5
    else if (counter == OFFSET + 165) scl_reg <= 0;
    else if (counter == OFFSET + 166) scl_reg <= 1; // Data  4
    else if (counter == OFFSET + 167) scl_reg <= 0;
    else if (counter == OFFSET + 168) scl_reg <= 1; // Data  3
    else if (counter == OFFSET + 169) scl_reg <= 0;
    else if (counter == OFFSET + 170) scl_reg <= 1; // Data  2
    else if (counter == OFFSET + 171) scl_reg <= 0;
    else if (counter == OFFSET + 172) scl_reg <= 1; // Data  1
    else if (counter == OFFSET + 173) scl_reg <= 0;
    else if (counter == OFFSET + 174) scl_reg <= 1; // Data  0
    else if (counter == OFFSET + 175) scl_reg <= 0;
    else if (counter == OFFSET + 176) scl_reg <= 1; // ACK
    else if (counter == OFFSET + 177) scl_reg <= 0;
    else if (counter == OFFSET + 178) scl_reg <= 0;

    else scl_reg <= 1;
end
    
always @(posedge i2c_clk_reg) begin
if (counter == OFFSET + 0) sda_reg <= 1;
    else if (counter == OFFSET + 1 || counter == OFFSET + 2) sda_reg <= 0;          // START

    else if (counter == OFFSET + 3 || counter == OFFSET + 4) sda_reg <= addr[0];    // Addr 6
    else if (counter == OFFSET + 5 || counter == OFFSET + 6) sda_reg <= addr[1];    // Addr 5
    else if (counter == OFFSET + 7 || counter == OFFSET + 8) sda_reg <= addr[2];    // Addr 4
    else if (counter == OFFSET + 9 || counter == OFFSET + 10) sda_reg <= addr[3];   // Addr 3
    else if (counter == OFFSET + 11 || counter == OFFSET + 12) sda_reg <= addr[4];  // Addr 2
    else if (counter == OFFSET + 13 || counter == OFFSET + 14) sda_reg <= addr[5];  // Addr 1
    else if (counter == OFFSET + 15 || counter == OFFSET + 16) sda_reg <= addr[6];  // Addr 0
    else if (counter == OFFSET + 17 || counter == OFFSET + 18) sda_reg <= 0;        // WR
    else if (counter == OFFSET + 19 || counter == OFFSET + 20) sda_reg <= 1'bz;     // ACK

    else if (counter == OFFSET + 21 || counter == OFFSET + 22) sda_reg <= 1; 
    else if (counter == OFFSET + 23) sda_reg <= command_config[7];

    else if (counter == OFFSET + 24 || counter == OFFSET + 25) sda_reg <= command_config[0]; // Data  7
    else if (counter == OFFSET + 26 || counter == OFFSET + 27) sda_reg <= command_config[1]; // Data  6
    else if (counter == OFFSET + 28 || counter == OFFSET + 29) sda_reg <= command_config[2]; // Data  5
    else if (counter == OFFSET + 30 || counter == OFFSET + 31) sda_reg <= command_config[3]; // Data  4
    else if (counter == OFFSET + 32 || counter == OFFSET + 33) sda_reg <= command_config[4]; // Data  3
    else if (counter == OFFSET + 34 || counter == OFFSET + 35) sda_reg <= command_config[5]; // Data  2
    else if (counter == OFFSET + 36 || counter == OFFSET + 37) sda_reg <= command_config[6]; // Data  1
    else if (counter == OFFSET + 38 || counter == OFFSET + 39) sda_reg <= command_config[7]; // Data  0
    else if (counter == OFFSET + 40 || counter == OFFSET + 41) sda_reg <= 1'bz;              // ACK

    else if (counter == OFFSET + 42 || counter == OFFSET + 43) sda_reg <= 1;
    else if (counter == OFFSET + 44) sda_reg <= config_port0[7];

    else if (counter == OFFSET + 45 || counter == OFFSET + 46) sda_reg <= config_port0[0]; // Data  7
    else if (counter == OFFSET + 47 || counter == OFFSET + 48) sda_reg <= config_port0[1]; // Data  6
    else if (counter == OFFSET + 49 || counter == OFFSET + 50) sda_reg <= config_port0[2]; // Data  5
    else if (counter == OFFSET + 51 || counter == OFFSET + 52) sda_reg <= config_port0[3]; // Data  4
    else if (counter == OFFSET + 53 || counter == OFFSET + 54) sda_reg <= config_port0[4]; // Data  3
    else if (counter == OFFSET + 55 || counter == OFFSET + 56) sda_reg <= config_port0[5]; // Data  2
    else if (counter == OFFSET + 57 || counter == OFFSET + 58) sda_reg <= config_port0[6]; // Data  1
    else if (counter == OFFSET + 59 || counter == OFFSET + 60) sda_reg <= config_port0[7]; // Data  0
    else if (counter == OFFSET + 61 || counter == OFFSET + 62) sda_reg <= 1'bz;            // ACK

    else if (counter == OFFSET + 63 || counter == OFFSET + 64) sda_reg <= 1;
    else if (counter == OFFSET + 65) sda_reg <= config_port1[7];

    else if (counter == OFFSET + 66 || counter == OFFSET + 67) sda_reg <= config_port1[0]; // Data  7
    else if (counter == OFFSET + 68 || counter == OFFSET + 69) sda_reg <= config_port1[1]; // Data  6
    else if (counter == OFFSET + 70 || counter == OFFSET + 71) sda_reg <= config_port1[2]; // Data  5
    else if (counter == OFFSET + 72 || counter == OFFSET + 73) sda_reg <= config_port1[3]; // Data  4
    else if (counter == OFFSET + 74 || counter == OFFSET + 75) sda_reg <= config_port1[4]; // Data  3
    else if (counter == OFFSET + 76 || counter == OFFSET + 77) sda_reg <= config_port1[5]; // Data  2
    else if (counter == OFFSET + 78 || counter == OFFSET + 79) sda_reg <= config_port1[6]; // Data  1
    else if (counter == OFFSET + 80 || counter == OFFSET + 81) sda_reg <= config_port1[7]; // Data  0
    else if (counter == OFFSET + 82 || counter == OFFSET + 83) sda_reg <= 1'bz;            // ACK

    else if (counter == OFFSET + 84 || counter == OFFSET + 85 || counter == OFFSET + 86) sda_reg <= 0;

    else if (counter == OFFSET + 93) sda_reg <= 1;
    else if (counter == OFFSET + 94 || counter == OFFSET + 95) sda_reg <= 0;   // START

    else if (counter == OFFSET + 96 || counter == OFFSET + 97) sda_reg <= addr[0];    // Addr 6
    else if (counter == OFFSET + 98 || counter == OFFSET + 99) sda_reg <= addr[1];    // Addr 5
    else if (counter == OFFSET + 100 || counter == OFFSET + 101) sda_reg <= addr[2];  // Addr 4
    else if (counter == OFFSET + 102 || counter == OFFSET + 103) sda_reg <= addr[3];  // Addr 3
    else if (counter == OFFSET + 104 || counter == OFFSET + 105) sda_reg <= addr[4];  // Addr 2
    else if (counter == OFFSET + 106 || counter == OFFSET + 107) sda_reg <= addr[5];  // Addr 1
    else if (counter == OFFSET + 108 || counter == OFFSET + 109) sda_reg <= addr[6];  // Addr 0
    else if (counter == OFFSET + 110 || counter == OFFSET + 111) sda_reg <= 0;        // WR
    else if (counter == OFFSET + 112 || counter == OFFSET + 113) sda_reg <= 1'bz;     // ACK

    else if (counter == OFFSET + 114 || counter == OFFSET + 115) sda_reg <= 1; 
    else if (counter == OFFSET + 116) sda_reg <= command_output[7];

    else if (counter == OFFSET + 117 || counter == OFFSET + 118) sda_reg <= command_output[0]; // Data  7
    else if (counter == OFFSET + 119 || counter == OFFSET + 120) sda_reg <= command_output[1]; // Data  6
    else if (counter == OFFSET + 121 || counter == OFFSET + 122) sda_reg <= command_output[2]; // Data  5
    else if (counter == OFFSET + 123 || counter == OFFSET + 124) sda_reg <= command_output[3]; // Data  4
    else if (counter == OFFSET + 125 || counter == OFFSET + 126) sda_reg <= command_output[4]; // Data  3
    else if (counter == OFFSET + 127 || counter == OFFSET + 128) sda_reg <= command_output[5]; // Data  2
    else if (counter == OFFSET + 129 || counter == OFFSET + 130) sda_reg <= command_output[6]; // Data  1
    else if (counter == OFFSET + 131 || counter == OFFSET + 132) sda_reg <= command_output[7]; // Data  0
    else if (counter == OFFSET + 133 || counter == OFFSET + 134) sda_reg <= 1'bz;              // ACK

    else if (counter == OFFSET + 135 || counter == OFFSET + 136) sda_reg <= 1;
    else if (counter == OFFSET + 137) sda_reg <= output_port0[7];

    else if (counter == OFFSET + 138 || counter == OFFSET + 139) sda_reg <= output_port0[0]; // Data  7
    else if (counter == OFFSET + 140 || counter == OFFSET + 141) sda_reg <= output_port0[1]; // Data  6
    else if (counter == OFFSET + 142 || counter == OFFSET + 143) sda_reg <= output_port0[2]; // Data  5
    else if (counter == OFFSET + 144 || counter == OFFSET + 145) sda_reg <= output_port0[3]; // Data  4
    else if (counter == OFFSET + 146 || counter == OFFSET + 147) sda_reg <= output_port0[4]; // Data  3 
    else if (counter == OFFSET + 148 || counter == OFFSET + 149) sda_reg <= output_port0[5]; // Data  2
    else if (counter == OFFSET + 150 || counter == OFFSET + 151) sda_reg <= output_port0[6]; // Data  1
    else if (counter == OFFSET + 152 || counter == OFFSET + 153) sda_reg <= output_port0[7]; // Data  0
    else if (counter == OFFSET + 154 || counter == OFFSET + 155) sda_reg <= 1'bz;              // ACK

    else if (counter == OFFSET + 156 || counter == OFFSET + 157) sda_reg <= 1;
    else if (counter == OFFSET + 158) sda_reg <= output_port1[7];

    else if (counter == OFFSET + 159 || counter == OFFSET + 160) sda_reg <= output_port1[0]; // Data  7
    else if (counter == OFFSET + 161 || counter == OFFSET + 162) sda_reg <= output_port1[1]; // Data  6
    else if (counter == OFFSET + 163 || counter == OFFSET + 164) sda_reg <= output_port1[2]; // Data  5
    else if (counter == OFFSET + 165 || counter == OFFSET + 166) sda_reg <= output_port1[3]; // Data  4
    else if (counter == OFFSET + 167 || counter == OFFSET + 168) sda_reg <= output_port1[4]; // Data  3 
    else if (counter == OFFSET + 169 || counter == OFFSET + 170) sda_reg <= output_port1[5]; // Data  2
    else if (counter == OFFSET + 171 || counter == OFFSET + 172) sda_reg <= output_port1[6]; // Data  1
    else if (counter == OFFSET + 173 || counter == OFFSET + 174) sda_reg <= output_port1[7]; // Data  0
    else if (counter == OFFSET + 175 || counter == OFFSET + 176) sda_reg <= 1'bz;              // ACK

    else if (counter == OFFSET + 177 || counter == OFFSET + 178 || counter == OFFSET + 179) sda_reg <= 0; // STOP

    else sda_reg <= 1;
end

//assign scl = scl_reg;
//assign sda = sda_reg;

endmodule