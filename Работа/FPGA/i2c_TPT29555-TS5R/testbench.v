`timescale 1ns / 1ps

module testbench ();
    
	// DUT INPUTS
    reg [0:0] mclk;    // Input System Clock 24.576 MHz
    reg [0:0] aresetn;
    
    // I2C
    wire i2c_clk;
    wire scl;
    wire sda;
    
    i2c_master MASTER(i2c_clk, aresetn, scl, sda);
    i2c_clk_gen CLKGEN(mclk, aresetn, i2c_clk);

    // generate a 100 MHz periodic clock waveform
    always
        #1 mclk <= ~mclk;

    // assign inputs at various times
    initial 
    begin
        mclk <= 1'b0;
        aresetn <= 1'b1;
        #10 aresetn <= 1'b0;
        #20 aresetn <= 1'b1;
    end
    
endmodule
