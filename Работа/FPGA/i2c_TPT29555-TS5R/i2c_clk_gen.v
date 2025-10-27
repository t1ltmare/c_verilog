module i2c_clk_gen
(
    input  mclk, // 24.576 MHz
    input  aresetn,
    //-------------
    output reg i2c_clk // ~800 kHz (scl будет работать по фронту, поэтому частота будет вдвое меньше, 400 кГц)
);

reg [4:0] clk_counter;          
reg [0:0] i2c_clk_reg;           // i2c clk 800 kHz (24.576 MHz / 32 = 768 kHz) (scl = i2c_clk / 2 = 384 kHz)

always @(posedge mclk or negedge aresetn) begin
    if (!aresetn) begin
        clk_counter <= 0;
        i2c_clk_reg <= 0;
    end else begin
        if (clk_counter == 63) begin
            clk_counter <= 0;
        end else begin
            clk_counter <= clk_counter + 1;
        end
        i2c_clk_reg <= clk_counter[4];
    end
end

assign i2c_clk = i2c_clk_reg;

endmodule