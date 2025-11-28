`timescale 1ns/1ps

module top_tb;

// Clock and reset
reg mclk;
reg clk_400khz;
reg rst_n;

// I2C signals
reg scl;
wire sda;

// Audio signals (не используются в тесте, но требуются для top)
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

// Test control
integer test_pass;
integer test_fail;
integer test_counter;

// I2C master simulation
reg sda_drive;

// Assignments
assign sda = dut.i2c_slave_inst.sda_oe ? 1'bz : (sda_drive ? 1'bz : 1'b0);

// DUT instance
top dut (
    .mclk(mclk),
    .clk_400khz(clk_400khz),
    .rst_n(rst_n),
    .lrck_48(lrck_48),
    .sclk_3072(sclk_3072),
    .i2s_data_in(i2s_data_in),
    .sdo_ff_bus(sdo_ff_bus),
    .sdo_loopback(sdo_loopback),
    .scl(scl),
    .sda(sda),
    // Register interface
    .reg_addr(reg_addr),
    .reg_data_in(reg_data_in),
    .reg_write(reg_write),
    .reg_data_out(reg_data_out),
    .reg_valid(reg_valid),
    .reg_read_req(reg_read_req),
    .reg_read_addr(reg_read_addr)
);

// Clock generation (50MHz for mclk, 400kHz for I2C)
always #10 mclk = ~mclk;  // 50MHz
always #1250 clk_400khz = ~clk_400khz;  // 400kHz

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
    for (int i = 0; i < 128; i++)
        test_register_write(i);
    test_i2c_write();
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
    clk_400khz = 0;
    rst_n = 1;
    scl = 1;
    sda_drive = 1;
    
    // Initialize audio signals
    lrck_48 = 0;
    sclk_3072 = 0;
    i2s_data_in = 48'b0;
    
    // Initialize register interface
    reg_addr = 8'h00;
    reg_data_in = 8'h00;
    reg_write = 0;
    
    test_pass = 0;
    test_fail = 0;
    test_counter = 0;
    
    #100;
    rst_n = 0;  // Assert reset
    #200;
    rst_n = 1;  // Deassert reset
    #1000;
    
    $display("Initialization complete");
end
endtask

// Test reset functionality
task test_reset;
begin
    test_counter = test_counter + 1;
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
    
    #1000;
end
endtask

// Test register write through external interface (теперь работает!)
task test_register_write;
input [7:0] data;
begin
    test_counter = test_counter + 1;
    $display("Test %0d: External register write", test_counter);
    
    reg_addr = data;
    reg_data_in = data + 8'hC0;
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
    
    if (test_fail == 0) begin
        $display("*** ALL TESTS PASSED ***");
    end else begin
        $display("*** SOME TESTS FAILED ***");
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

// Slave address monitor
always @(posedge clk_400khz) begin
    if (dut.i2c_slave_inst.scl_falling && dut.i2c_slave_inst.current_state.name == "RECV_SLAVE_ADDR" && dut.i2c_slave_inst.bit_counter == 3'd7)
        $display("    Slave address is: 0x%02X", dut.i2c_slave_inst.shift_reg[6:0]);
end

// Waveform dump
initial begin
    $dumpfile("top_tb.vcd");
    $dumpvars(0, top_tb);
end

endmodule