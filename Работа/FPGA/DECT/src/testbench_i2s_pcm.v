`timescale 1ns / 1ps

module i2s_system_tb;

    // --- Signals ---
    // Clock and control signals
    // reg clk;                 // System clock (for the receiver - можно не использовать)  - Removed, since generator doesn't use clk

    // I2S Generator outputs
    wire i2s1_data_gen;
    wire i2s2_data_gen;
    wire sclk;
    wire lrck;
    reg clk;

    // --- Instantiation ---
    // I2S Generator
    i2s_generator uut_gen (
        .clk(clk),
        .sclk(sclk),
        .lrck(lrck),
        .i2s1_data(i2s1_data_gen),
        .i2s2_data(i2s2_data_gen)
    );

    // --- Parameters ---
    parameter CLK_PERIOD = 40;                 // CLK period in ns (10 MHz)

    // Generate LRCK. LRCK is low for 32 sclk cycles, then high for 32 sclk cycles
    initial begin
        #1;  
        clk = 0;
    end

    // Generate CLK
    always #(CLK_PERIOD/2) clk = ~clk;

endmodule