# ModelSim simulation script for I2C Slave without vopt

# Load design without optimization
vsim -t ps work.i2c_slave_tb

# Enable acceleration
vsim -t ps +acc work.i2c_slave_tb

# Add waves to waveform window
add wave -noupdate -divider "Clock and Reset"
add wave -color yellow clk
add wave -color yellow rst_n

add wave -noupdate -divider "I2C Physical Interface"
add wave -color cyan scl
add wave -color cyan sda
add wave -color cyan sda_drive

add wave -noupdate -divider "Register Interface"
add wave -color white reg_addr
add wave -color white reg_data_in
add wave -color white reg_write
add wave -color white reg_data_out
add wave -color white reg_valid
add wave -color white reg_read_req
add wave -color white reg_read_addr

add wave -noupdate -divider "Slave Internal State"
add wave -color magenta /i2c_slave_tb/dut/current_state
add wave -color magenta /i2c_slave_tb/dut/next_state
add wave -color magenta /i2c_slave_tb/dut/shift_reg
add wave -color magenta /i2c_slave_tb/dut/bit_counter
add wave -color magenta /i2c_slave_tb/dut/address_matched
add wave -color magenta /i2c_slave_tb/dut/read_mode
add wave -color magenta /i2c_slave_tb/dut/current_reg_addr
add wave -color magenta /i2c_slave_tb/dut/sda_out
add wave -color magenta /i2c_slave_tb/dut/sda_oe

add wave -noupdate -divider "Synchronization"
add wave -color orange /i2c_slave_tb/dut/sda_sync
add wave -color orange /i2c_slave_tb/dut/scl_sync
add wave -color orange /i2c_slave_tb/dut/sda_falling
add wave -color orange /i2c_slave_tb/dut/scl_falling
add wave -color orange /i2c_slave_tb/dut/scl_rising

add wave -noupdate -divider "File Registers"
add wave -color orange -radix hex /i2c_slave_tb/dut/register_file

add wave -noupdate -divider "Test Status"
add wave -color green /i2c_slave_tb/test_pass
add wave -color red /i2c_slave_tb/test_fail
add wave -color blue /i2c_slave_tb/test_counter

# Set wave formatting
configure wave -namecolwidth 200
configure wave -valuecolwidth 80
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2

# Run simulation
run 500us

echo "========================================="
echo "I2C Slave simulation completed!"
echo "========================================="

wave zoom full