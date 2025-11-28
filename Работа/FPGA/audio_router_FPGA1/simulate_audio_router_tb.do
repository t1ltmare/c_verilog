# ModelSim simulation script for Top Module

# Load design without optimization
vsim -t ps work.top_tb

# Load design
vsim -t ps +acc work.top_tb

# Add waves to waveform window
add wave -noupdate -divider "Clock and Reset"
add wave -color yellow mclk
add wave -color yellow rst_n

add wave -noupdate -divider "Audio Clocks"
add wave -color cyan /top_tb/dut/audio_router_inst/lrck_48
add wave -color cyan /top_tb/dut/audio_router_inst/lrck_48_sync
add wave -color cyan /top_tb/dut/audio_router_inst/lrck_48_rise
add wave -color cyan /top_tb/dut/audio_router_inst/lrck_48_fall
add wave -color cyan /top_tb/dut/audio_router_inst/sclk_3072
add wave -color cyan /top_tb/dut/audio_router_inst/sclk_3072
add wave -color cyan /top_tb/dut/audio_router_inst/sclk_3072_counter

add wave -color magenta /top_tb/dut/audio_router_inst/sclk_12288
add wave -color magenta /top_tb/dut/audio_router_inst/lrck_192
add wave -color magenta /top_tb/dut/audio_router_inst/lrck_192_sync
add wave -color magenta /top_tb/dut/audio_router_inst/lrck_192_rise
add wave -color magenta /top_tb/dut/audio_router_inst/lrck_192_fall

add wave -color cyan /top_tb/lrck_48
add wave -color cyan /top_tb/sclk_3072
add wave -color magenta /top_tb/sclk_12288
add wave -color magenta /top_tb/lrck_192

add wave -noupdate -divider "I2S Input Data"
add wave -color white /top_tb/i2s_data_in
add wave -color green -radix hex /top_tb/dut/audio_router_inst/channel_buffer

add wave -noupdate -divider "FF, LoopBack buffer"
add wave -color green -radix hex /top_tb/dut/audio_router_inst/ff_channels
add wave -color magenta /top_tb/dut/audio_router_inst/ff_channel_index
add wave -color magenta /top_tb/dut/audio_router_inst/ff_bit_index
add wave -color green -radix hex /top_tb/dut/audio_router_inst/loopback_channels

add wave -noupdate -divider "Outputs"
add wave -color white -radix binary /top_tb/sdo_ff_bus
add wave -color white -radix binary /top_tb/sdo_loopback

add wave -noupdate -divider "I2C Signals"
add wave -color cyan /top_tb/scl
add wave -color cyan /top_tb/sda
add wave -color cyan /top_tb/sda_drive

add wave -noupdate -divider "Register Interface"
add wave -color white reg_addr
add wave -color white reg_data_in
add wave -color white reg_write
add wave -color white reg_data_out
add wave -color white reg_valid
add wave -color white reg_read_req
add wave -color white reg_read_addr

add wave -noupdate -divider "I2C Slave Internal"
add wave -color magenta /top_tb/dut/i2c_slave_inst/current_state
add wave -color magenta /top_tb/dut/i2c_slave_inst/next_state
add wave -color magenta -radix hex /top_tb/dut/i2c_slave_inst/shift_reg
add wave -color magenta /top_tb/dut/i2c_slave_inst/bit_counter
add wave -color magenta /top_tb/dut/i2c_slave_inst/address_matched
add wave -color magenta /top_tb/dut/i2c_slave_inst/read_mode
add wave -color magenta -radix hex /top_tb/dut/i2c_slave_inst/current_reg_addr
add wave -color magenta /top_tb/dut/i2c_slave_inst/sda_out
add wave -color magenta /top_tb/dut/i2c_slave_inst/sda_oe

add wave -noupdate -divider "Synchronization"
add wave -color orange /top_tb/dut/i2c_slave_inst/sda_sync
add wave -color orange /top_tb/dut/i2c_slave_inst/scl_sync
add wave -color orange /top_tb/dut/i2c_slave_inst/sda_falling
add wave -color orange /top_tb/dut/i2c_slave_inst/scl_falling
add wave -color orange /top_tb/dut/i2c_slave_inst/scl_rising

add wave -noupdate -divider "Register File"
add wave -color orange -radix hex /top_tb/dut/register_file

add wave -noupdate -divider "Test I2C Status"
add wave -color green /top_tb/test_pass
add wave -color red /top_tb/test_fail
add wave -color blue /top_tb/test_counter

add wave -noupdate -divider "Test I2S Status"
add wave -color green /top_tb/test_router_pass
add wave -color red /top_tb/test_router_fail
add wave -color blue /top_tb/test_router_counter

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
run 200000us

echo "========================================="
echo "Simulation completed!"
echo "========================================="

wave zoom full