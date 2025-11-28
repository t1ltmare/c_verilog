onerror {resume}
quietly WaveActivateNextPane {} 0

add wave -noupdate -divider {Clock and Reset}
add wave -noupdate -color yellow /top_tb/mclk
add wave -noupdate -color yellow /top_tb/clk_400khz
add wave -noupdate -color yellow /top_tb/rst_n

add wave -noupdate -divider {Audio Clocks}
add wave -noupdate -color cyan /top_tb/lrck_48
add wave -noupdate -color cyan /top_tb/sclk_3072
add wave -noupdate -color cyan /top_tb/dut/audio_router_inst/sclk_12288
add wave -noupdate -color cyan /top_tb/dut/audio_router_inst/lrck_192

add wave -noupdate -divider {I2S Input Data}
add wave -noupdate -color white -radix hexadecimal /top_tb/i2s_data_in

add wave -noupdate -divider {Outputs}
add wave -noupdate -color white -radix binary /top_tb/sdo_ff_bus
add wave -noupdate -color white -radix binary /top_tb/sdo_loopback

add wave -noupdate -divider {I2C Signals}
add wave -noupdate -color cyan /top_tb/scl
add wave -noupdate -color cyan /top_tb/sda
add wave -noupdate -color cyan /top_tb/sda_drive

add wave -noupdate -divider {I2C Slave Internal}
add wave -noupdate -color magenta /top_tb/dut/i2c_slave_inst/current_state
add wave -noupdate -color magenta /top_tb/dut/i2c_slave_inst/next_state
add wave -noupdate -color magenta -radix hexadecimal /top_tb/dut/i2c_slave_inst/shift_reg
add wave -noupdate -color magenta /top_tb/dut/i2c_slave_inst/bit_counter
add wave -noupdate -color magenta /top_tb/dut/i2c_slave_inst/address_matched
add wave -noupdate -color magenta /top_tb/dut/i2c_slave_inst/read_mode
add wave -noupdate -color magenta -radix hexadecimal /top_tb/dut/i2c_slave_inst/current_reg_addr
add wave -noupdate -color magenta /top_tb/dut/i2c_slave_inst/sda_out
add wave -noupdate -color magenta /top_tb/dut/i2c_slave_inst/sda_oe

add wave -noupdate -divider {Register File}
add wave -noupdate -color orange -radix hexadecimal /top_tb/dut/register_file

add wave -noupdate -divider {Audio Router Internal}
add wave -noupdate -color green /top_tb/dut/audio_router_inst/mclk
add wave -noupdate -color green /top_tb/dut/audio_router_inst/lrck_48_rise
add wave -noupdate -color green /top_tb/dut/audio_router_inst/lrck_48_fall
add wave -noupdate -color green /top_tb/dut/audio_router_inst/lrck_192_rise
add wave -noupdate -color green /top_tb/dut/audio_router_inst/lrck_192_fall

add wave -noupdate -divider {Channel Buffers}
add wave -noupdate -color green -radix hexadecimal /top_tb/dut/audio_router_inst/channel_buffer

add wave -noupdate -divider {Output Channels}
add wave -noupdate -color green -radix hexadecimal /top_tb/dut/audio_router_inst/ff_channels
add wave -noupdate -color green -radix hexadecimal /top_tb/dut/audio_router_inst/loopback_channels

add wave -noupdate -divider {Output Counters}
add wave -noupdate -color blue -radix unsigned /top_tb/dut/audio_router_inst/ff_channel_index
add wave -noupdate -color blue -radix unsigned /top_tb/dut/audio_router_inst/ff_bit_index
add wave -noupdate -color blue -radix unsigned /top_tb/dut/audio_router_inst/loopback_channel_sel
add wave -noupdate -color blue -radix unsigned /top_tb/dut/audio_router_inst/loopback_bit_index

add wave -noupdate -divider {Test Status}
add wave -noupdate -color green /top_tb/test_pass
add wave -noupdate -color red /top_tb/test_fail
add wave -noupdate -color blue /top_tb/test_counter

TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {30000000 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 250
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1000
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {0 ps} {21000 us}