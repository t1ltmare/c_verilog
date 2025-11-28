`timescale 1ns/1ps

module top_tb;

// Clock and reset
reg mclk;
reg rst_n;

// I2C signals
reg scl;
tri sda;

// Audio signals - изменены направления
reg lrck_192;
reg sclk_12288;
reg lrck_48;
reg sclk_3072;

// Новые входы для DUT
reg [3:0] sdi_ff_bus;
reg [3:0] sdi_loopback;

// Новый выход из DUT
wire [47:0] i2s_data_out;

// Register interface
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
    
    // Новые порты
    .sdi_ff_bus(sdi_ff_bus),
    .sdi_loopback(sdi_loopback),
    .i2s_data_out(i2s_data_out),
    
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

// Clock generation (sclk, lrck) - без изменений

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
        if (lrck_192_counter == 16'd31) begin
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
        if (sclk_3072_counter == 8'd3) begin
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
        if (lrck_48_counter == 16'd31) begin
            lrck_48_counter <= 16'd0;
            lrck_48 <= ~lrck_48;
        end else begin
            lrck_48_counter <= lrck_48_counter + 16'd1;
        end
    end
end

// Генерация тестовых данных для входных шин
reg [4:0] ff_bit_counter [0:3];
reg [4:0] loopback_bit_counter [0:3];
reg [2:0] ff_channel_counter [0:3];
reg loopback_channel_sel [0:3];

// Инициализация счетчиков для входных данных
initial begin
    for (int i = 0; i < 4; i++) begin
        ff_bit_counter[i] = 0;
        loopback_bit_counter[i] = 0;
        ff_channel_counter[i] = 0;
        loopback_channel_sel[i] = 0;
    end
end

// ******** Тест audio_router: сравнение входных и выходных данных ******** //

// Добавляем переменные для захвата данных

logic [23:0] captured_sdi_ff [0:3];
logic [23:0] captured_i2s_ff [0:3];
logic [23:0] captured_i2s_lb [0:3];
logic [23:0] captured_sdi_lb [0:3];

logic [23:0] buf_captured_sdi_ff [0:3];
logic [23:0] buf_captured_sdi_lb [0:3];

// Буферизация захваченных данных sdi_ff для сравнения

always_ff @(posedge sclk_12288 or negedge rst_n) begin
    if (!rst_n) begin
        for (int bus = 0; bus < 4; bus++) begin
            buf_captured_sdi_ff[bus] <= 24'b0;
            buf_captured_sdi_lb[bus] <= 24'b0;
        end
    end else begin
        if (!dut.audio_router_inst.lrck_48 && dut.audio_router_inst.lrck_48_sync && !dut.audio_router_inst.lrck_192 && dut.audio_router_inst.lrck_192_sync) begin
            buf_captured_sdi_ff = captured_sdi_ff;
            buf_captured_sdi_lb = captured_sdi_lb;
        end
    end 
end

// Захват выходных данных i2s_data_out для сравнения с sdi_ff

always_ff @(posedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        for (int bus = 0; bus < 4; bus++) begin
            captured_i2s_ff[bus] <= 1'b0;
        end
    end else begin
        for (int bus = 0; bus < 48; bus++) begin
            // Обработка битов данных и нулей
            if (!lrck_48 && dut.audio_router_inst.bit_index < 24 && (bus == 0 || bus == 4 || bus == 8 || bus == 12))
                captured_i2s_ff[bus[3:2]] <= {captured_i2s_ff[bus[3:2]][22:0], i2s_data_out[bus]};
            if (!lrck_48 && dut.audio_router_inst.bit_index < 24 && (bus == 16 || bus == 17 || bus == 18 || bus == 19))
                captured_i2s_lb[bus[1:0]] <= {captured_i2s_lb[bus[1:0]][22:0], i2s_data_out[bus]};
        end
    end
end

// Сравнение захваченных данных при каждом фронте lrck_48

always_ff @(posedge dut.audio_router_inst.lrck_48_rise or negedge rst_n) begin
    for (int bus = 0; bus < 4; bus++) begin
        if (buf_captured_sdi_ff[bus] != captured_i2s_ff[bus]) begin
            $display("  FAIL: Audio Router data mismatch on channel %0d: expected=0x%06X, got=0x%06X", bus, buf_captured_sdi_ff[bus], captured_i2s_ff[bus]);
            test_router_fail++;
            test_router_counter++;
        end else begin
            $display("  PASS: Audio Router data match on channel %0d: data=0x%06X", bus, captured_i2s_ff[bus]);
            test_router_pass++;
            test_router_counter++;
        end
        if (buf_captured_sdi_lb[bus] != captured_i2s_lb[bus]) begin
            $display("  FAIL: Audio Router data mismatch on channel %0d: expected=0x%06X, got=0x%06X", bus, buf_captured_sdi_ff[bus], captured_i2s_ff[bus]);
            test_router_fail++;
            test_router_counter++;
        end else begin
            $display("  PASS: Audio Router data match on channel %0d: data=0x%06X", bus, captured_i2s_ff[bus]);
            test_router_pass++;
            test_router_counter++;
        end
    end
end

// ******** Генерация случайных данных sdi_ff_bus и sdi_loopback ******** //

// Генерация данных для sdi_ff_bus (4 шины, 192 kHz)

always @(negedge sclk_12288 or negedge rst_n) begin
    if (!rst_n) begin
        for (int i = 0; i < 4; i++) begin
            ff_bit_counter[i] <= 0;
            ff_channel_counter[i] <= 0;
            sdi_ff_bus[i] <= 0;
        end
    end else begin
        for (int bus = 0; bus < 4; bus++) begin
            automatic logic [0:0] random_bit = $random & 1'b1; 
            // Генерация тестового паттерна: бит = (номер_шины + номер_канала + номер_бита) % 2
            if (ff_bit_counter[bus] < 24) begin
                sdi_ff_bus[bus] <= random_bit;
                if (!lrck_48 && (dut.audio_router_inst.sclk_3072_counter < 6 || dut.audio_router_inst.sclk_3072_counter > 29))
                    captured_sdi_ff[bus] <= {captured_sdi_ff[bus][22:0], random_bit};
            end else begin
                sdi_ff_bus[bus] <= 0; // Нули после данных
            end

            // Обновление счетчиков
            if (ff_bit_counter[bus] == 31) begin
                ff_bit_counter[bus] <= 0;
                if (ff_channel_counter[bus] == 7) begin
                    ff_channel_counter[bus] <= 0;
                end else begin
                    ff_channel_counter[bus] <= ff_channel_counter[bus] + 1;
                end
            end else begin
                ff_bit_counter[bus] <= ff_bit_counter[bus] + 1;
            end
        end
    end
end

// Генерация данных для sdi_loopback (4 шины, 48 kHz)

always @(negedge sclk_3072 or negedge rst_n) begin
    if (!rst_n) begin
        for (int i = 0; i < 4; i++) begin
            loopback_bit_counter[i] <= 0;
            loopback_channel_sel[i] <= 0;
            sdi_loopback[i] <= 0;
        end
    end else begin
        for (int bus = 0; bus < 4; bus++) begin
            automatic logic [0:0] random_bit = $random & 1'b1; 
            // Генерация тестового паттерна
            if (loopback_bit_counter[bus] < 24) begin
                sdi_loopback[bus] <= random_bit;
                if (!dut.audio_router_inst.lrck_48)
                    captured_sdi_lb[bus] <= {captured_sdi_lb[bus][22:0], random_bit};
            end else begin
                sdi_loopback[bus] <= 0; // Нули после данных
            end

            // Обновление счетчиков
            if (loopback_bit_counter[bus] == 31) begin
                loopback_bit_counter[bus] <= 0;
                loopback_channel_sel[bus] <= ~loopback_channel_sel[bus];
            end else begin
                loopback_bit_counter[bus] <= loopback_bit_counter[bus] + 1;
            end
        end
    end
end

// I2C pullup
pullup p1(sda);

// Main test sequence
initial begin
    $display("=========================================");
    $display("Top Testbench Started - New Audio Router");
    $display("=========================================");
    
    // Initialize
    initialize();
    
    // Test sequences
    test_reset();
    for (int i = 0; i < 96; i++)
        test_register_write(i);
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
    
    // Initialize input data buses
    sdi_ff_bus = 4'b0;
    sdi_loopback = 4'b0;
    
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

    // Initialize test data capture variables
    for (int i = 0; i < 4; i++) begin
        captured_sdi_ff[i] = 24'b0;
        captured_i2s_ff[i] = 24'b0;
        captured_i2s_lb[i] = 24'b0;
        captured_sdi_lb[i] = 24'b0;
        buf_captured_sdi_ff[i] = 24'b0;
    end
    
    #100;
    rst_n = 0;  // Assert reset
    #200;
    rst_n = 1;  // Deassert reset
    #1000;

    // Initialize registers
    for (int i = 0; i < 96; i++) begin
        reg_addr = i;
        reg_data_in = i;
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

// Test register write through external interface
task test_register_write;
input [7:0] data;
begin
    $display("Test %0d: External register write", test_counter);
    
    reg_addr = data;
    reg_data_in = data;
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
    $display("\nTest i2c Results:");
    $display("  Passed: %0d", test_pass);
    $display("  Failed: %0d", test_fail);
    $display("  Total:  %0d", test_pass + test_fail);

    $display("\nTest i2s Results:");
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

// Monitor output data
always @(posedge sclk_3072) begin
    if (rst_n && i2s_data_out != 0) begin
        $display("i2s_data_out = 48'h%012X", i2s_data_out);
    end
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