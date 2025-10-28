module i2c_master
#(
	parameter OFFSET = 10
)
(
    input  i2c_clk, // 24.576 MHz
    input  aresetn,
    //-------------
    output reg scl,
    output reg sda
);
       
reg [7:0] counter;

always @(posedge i2c_clk, negedge aresetn) begin
    if (aresetn == 0) counter <= 0;
    else if (counter < 255) counter <= counter +1;
end

reg [0:0] scl_reg;
reg [0:0] sda_reg;

always @(posedge i2c_clk) begin
    if (counter == OFFSET + 3) scl_reg <= 0;
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

    else scl_reg <= 1;
end
    
always @(posedge i2c_clk) begin
    if (counter == OFFSET + 0) sda_reg <= 0;
    else if (counter == OFFSET + 1) sda_reg <= 0;   // ADDR 0100000
    else if (counter == OFFSET + 2) sda_reg <= 0;
    else if (counter == OFFSET + 3) sda_reg <= 0; // Addr 6
    else if (counter == OFFSET + 4) sda_reg <= 0;
    else if (counter == OFFSET + 5) sda_reg <= 1; // Addr 5
    else if (counter == OFFSET + 6) sda_reg <= 1;
    else if (counter == OFFSET + 7) sda_reg <= 0; // Addr 4
    else if (counter == OFFSET + 8) sda_reg <= 0;
    else if (counter == OFFSET + 9) sda_reg <= 0; // Addr 3
    else if (counter == OFFSET + 10) sda_reg <= 0;
    else if (counter == OFFSET + 11) sda_reg <= 0; // Addr 2
    else if (counter == OFFSET + 12) sda_reg <= 0;
    else if (counter == OFFSET + 13) sda_reg <= 0; // Addr 1
    else if (counter == OFFSET + 14) sda_reg <= 0;
    else if (counter == OFFSET + 15) sda_reg <= 0; // Addr 0
    else if (counter == OFFSET + 16) sda_reg <= 0;
    else if (counter == OFFSET + 17) sda_reg <= 0; // WR
    else if (counter == OFFSET + 18) sda_reg <= 0;
    else if (counter == OFFSET + 19) sda_reg <= 1'bz; // ACK
    else if (counter == OFFSET + 20) sda_reg <= 1'bz;

    else if (counter == OFFSET + 21) sda_reg <= 1;  // COMMAND PORT 00000110
    else if (counter == OFFSET + 22) sda_reg <= 1;
    else if (counter == OFFSET + 23) sda_reg <= 1;
    else if (counter == OFFSET + 24) sda_reg <= 0; // Data  7
    else if (counter == OFFSET + 25) sda_reg <= 0;
    else if (counter == OFFSET + 26) sda_reg <= 0; // Data  6
    else if (counter == OFFSET + 27) sda_reg <= 0;
    else if (counter == OFFSET + 28) sda_reg <= 0; // Data  5
    else if (counter == OFFSET + 29) sda_reg <= 0;
    else if (counter == OFFSET + 30) sda_reg <= 0; // Data  4
    else if (counter == OFFSET + 31) sda_reg <= 0;
    else if (counter == OFFSET + 32) sda_reg <= 0; // Data  3
    else if (counter == OFFSET + 33) sda_reg <= 0;
    else if (counter == OFFSET + 34) sda_reg <= 1; // Data  2
    else if (counter == OFFSET + 35) sda_reg <= 1;
    else if (counter == OFFSET + 36) sda_reg <= 1; // Data  1
    else if (counter == OFFSET + 37) sda_reg <= 1;
    else if (counter == OFFSET + 38) sda_reg <= 0; // Data  0
    else if (counter == OFFSET + 39) sda_reg <= 0;
    else if (counter == OFFSET + 40) sda_reg <= 1'bz; // ACK
    else if (counter == OFFSET + 41) sda_reg <= 1'bz;

    else if (counter == OFFSET + 42) sda_reg <= 1;  // CONFIGURATION PORT 11111110
    else if (counter == OFFSET + 43) sda_reg <= 1;
    else if (counter == OFFSET + 44) sda_reg <= 1;
    else if (counter == OFFSET + 45) sda_reg <= 1; // Data  7
    else if (counter == OFFSET + 46) sda_reg <= 1;
    else if (counter == OFFSET + 47) sda_reg <= 1; // Data  6
    else if (counter == OFFSET + 48) sda_reg <= 1;
    else if (counter == OFFSET + 49) sda_reg <= 1; // Data  5
    else if (counter == OFFSET + 50) sda_reg <= 1;
    else if (counter == OFFSET + 51) sda_reg <= 1; // Data  4
    else if (counter == OFFSET + 52) sda_reg <= 1;
    else if (counter == OFFSET + 53) sda_reg <= 1; // Data  3
    else if (counter == OFFSET + 54) sda_reg <= 1;
    else if (counter == OFFSET + 55) sda_reg <= 1; // Data  2
    else if (counter == OFFSET + 56) sda_reg <= 1;
    else if (counter == OFFSET + 57) sda_reg <= 1; // Data  1
    else if (counter == OFFSET + 58) sda_reg <= 1;
    else if (counter == OFFSET + 59) sda_reg <= 0; // Data  0
    else if (counter == OFFSET + 60) sda_reg <= 0;
    else if (counter == OFFSET + 61) sda_reg <= 1'bz; // ACK
    else if (counter == OFFSET + 62) sda_reg <= 1'bz;

    else if (counter == OFFSET + 63) sda_reg <= 1;  // OUTPUT PORT 11111110
    else if (counter == OFFSET + 64) sda_reg <= 1;
    else if (counter == OFFSET + 65) sda_reg <= 1;
    else if (counter == OFFSET + 66) sda_reg <= 1; // Data  7
    else if (counter == OFFSET + 67) sda_reg <= 1;
    else if (counter == OFFSET + 68) sda_reg <= 1; // Data  6
    else if (counter == OFFSET + 69) sda_reg <= 1;
    else if (counter == OFFSET + 70) sda_reg <= 1; // Data  5
    else if (counter == OFFSET + 71) sda_reg <= 1;
    else if (counter == OFFSET + 72) sda_reg <= 1; // Data  4
    else if (counter == OFFSET + 73) sda_reg <= 1;
    else if (counter == OFFSET + 74) sda_reg <= 1; // Data  3
    else if (counter == OFFSET + 75) sda_reg <= 1;
    else if (counter == OFFSET + 76) sda_reg <= 1; // Data  2
    else if (counter == OFFSET + 77) sda_reg <= 1;
    else if (counter == OFFSET + 78) sda_reg <= 1; // Data  1
    else if (counter == OFFSET + 79) sda_reg <= 1;
    else if (counter == OFFSET + 80) sda_reg <= 0; // Data  0
    else if (counter == OFFSET + 81) sda_reg <= 0;
    else if (counter == OFFSET + 82) sda_reg <= 1'bz; // ACK
    else if (counter == OFFSET + 83) sda_reg <= 1'bz;

    else if (counter == OFFSET + 84) sda_reg <= 0;
    else if (counter == OFFSET + 85) sda_reg <= 0;
    else if (counter == OFFSET + 86) sda_reg <= 0;

    else sda_reg <= 1;
end

assign scl = scl_reg;
assign sda = sda_reg;

endmodule