module top #(
    parameter I2C_ADDR = 7'h50,
    parameter REG_COUNT = 40
) (
    input  wire        mclk,
    input  wire        clk_400khz,
    input  wire        rst_n,
    input  wire        lrck_48,
    input  wire        sclk_3072,
    input  wire [47:0] i2s_data_in,
    input  wire        scl,
    inout  wire        sda,
    input  wire [7:0] reg_addr,
    input  wire [7:0] reg_data_in,
    input  wire reg_write,
    input wire [7:0] reg_data_out,
    input wire reg_valid,
    input wire reg_read_req,
    input wire [7:0] reg_read_addr,

    output logic [3:0]  sdo_ff_bus,
    output logic [3:0]  sdo_loopback
);

// I2C register file
logic [7:0] register_file [0:REG_COUNT-1];

// I2C Slave instance
i2c_slave #(
    .SLAVE_ADDRESS(I2C_ADDR),
    .REGISTERS_COUNT(REG_COUNT)
) i2c_slave_inst (
    .clk(clk_400khz),
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

// Audio Router instance
audio_router #(
    .REG_COUNT(REG_COUNT)
) audio_router_inst (
    .mclk(mclk),
    .rst_n(rst_n),
    .lrck_48(lrck_48),
    .sclk_3072(sclk_3072),
    .i2s_data_in(i2s_data_in),
    .sdo_ff_bus(sdo_ff_bus),
    .sdo_loopback(sdo_loopback),
    .register_file(register_file)
);

endmodule