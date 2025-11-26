# Complete I2C Slave simulation script without vopt

# Clean up
quit -sim
echo "Cleaning previous simulation..."

# Create library
vlib work
vmap work work

echo "========================================="
echo "Compiling I2C Slave Design"
echo "========================================="

# Compile design files
vlog -sv i2c_slave.v
vlog -sv i2c_slave_tb.v

echo "========================================="
echo "Starting Simulation"
echo "========================================="

# Start simulation with full access
vsim -t ps +acc i2c_slave_tb

# Add all important signals
add wave -noupdate -divider "Testbench-Control"
add wave /i2c_slave_tb/clk
add wave /i2c_slave_tb/rst_n
add wave /i2c_slave_tb/test_pass
add wave /i2c_slave_tb/test_fail

add wave -noupdate -divider "I2C-Signals"
add wave /i2c_slave_tb/scl
add wave /i2c_slave_tb/sda
add wave /i2c_slave_tb/sda_drive

add wave -noupdate -divider "Register-Interface"
add wave /i2c_slave_tb/reg_addr
add wave /i2c_slave_tb/reg_data_in
add wave /i2c_slave_tb/reg_write
add wave /i2c_slave_tb/reg_data_out
add wave /i2c_slave_tb/reg_valid
add wave /i2c_slave_tb/reg_read_req
add wave /i2c_slave_tb/reg_read_addr

add wave -noupdate -divider "Slave-FSM"
add wave /i2c_slave_tb/dut/current_state
add wave /i2c_slave_tb/dut/next_state
add wave /i2c_slave_tb/dut/bit_counter
add wave /i2c_slave_tb/dut/address_matched
add wave /i2c_slave_tb/dut/read_mode

add wave -noupdate -divider "Slave-Data"
add wave /i2c_slave_tb/dut/shift_reg
add wave /i2c_slave_tb/dut/current_reg_addr
add wave /i2c_slave_tb/dut/sda_out
add wave /i2c_slave_tb/dut/sda_oe

# Configure wave window
configure wave -namecolwidth 180
configure wave -valuecolwidth 70
configure wave -justifyvalue left

# Run simulation
run 200us

echo "========================================="
echo "Simulation Finished"
echo "========================================="

# Zoom to full view
wave zoom full