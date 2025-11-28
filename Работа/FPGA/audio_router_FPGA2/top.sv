module top #(
    parameter I2C_ADDR = 7'h50,
    parameter REG_COUNT = 96
) (
    input  wire        mclk,
    input  wire        rst_n,
    input  wire        lrck_192,
    input  wire        sclk_12288,
    
    // Измененные порты для нового audio_router
    input  wire [3:0]  sdi_ff_bus,        // 4 входные шины 192 kHz (были выходными)
    input  wire [3:0]  sdi_loopback,      // 4 входа на FPGA2 48 kHz (были выходными)
    output wire [47:0] i2s_data_out,      // 48 выходных линий I2S (новый выход)
    
    // I2C interface
    input  wire        scl,
    inout  wire        sda,
    
    // Register interface (оставляем как есть)
    input  wire [7:0]  reg_addr,
    input  wire [7:0]  reg_data_in,
    input  wire        reg_write,
    output wire [7:0]  reg_data_out,
    output wire        reg_valid,
    output wire        reg_read_req,
    output wire [7:0]  reg_read_addr
);

// I2C register file
logic [7:0] register_file [0:REG_COUNT-1];

// I2C Slave instance
i2c_slave #(
    .SLAVE_ADDRESS(I2C_ADDR),
    .REGISTERS_COUNT(REG_COUNT)
) i2c_slave_inst (
    .clk(mclk),
    .rst_n(rst_n),
    .scl(scl),
    .sda(sda),
    .reg_addr(reg_addr),
    .reg_data_in(reg_data_in),
    .reg_write(reg_write),
    .reg_data_out(reg_data_out),
    .reg_valid(reg_valid),
    .reg_read_req(reg_read_req),
    .reg_read_addr(reg_read_addr),
    .register_file(register_file)
);

// Audio Router instance с новыми портами
audio_router #(
    .REG_COUNT(REG_COUNT)
) audio_router_inst (
    .mclk(mclk),
    .rst_n(rst_n),
    .lrck_192(lrck_192),
    .sclk_12288(sclk_12288),
    
    // Новые входы
    .sdi_ff_bus(sdi_ff_bus),
    .sdi_loopback(sdi_loopback),
    
    // Новый выход
    .i2s_data_out(i2s_data_out),
    
    // Register file остается
    .register_file(register_file)
);

endmodule