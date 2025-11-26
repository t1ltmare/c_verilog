onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -divider {Clock and Reset}
add wave -noupdate -color yellow /i2c_slave_tb/clk
add wave -noupdate -color yellow /i2c_slave_tb/rst_n
add wave -noupdate -divider {I2C Physical Interface}
add wave -noupdate -color cyan /i2c_slave_tb/scl
add wave -noupdate -color cyan /i2c_slave_tb/sda
add wave -noupdate -color cyan /i2c_slave_tb/sda_drive
add wave -noupdate -divider {Register Interface}
add wave -noupdate -color white /i2c_slave_tb/reg_addr
add wave -noupdate -color white /i2c_slave_tb/reg_data_in
add wave -noupdate -color white /i2c_slave_tb/reg_write
add wave -noupdate -color white /i2c_slave_tb/reg_data_out
add wave -noupdate -color white /i2c_slave_tb/reg_valid
add wave -noupdate -color white /i2c_slave_tb/reg_read_req
add wave -noupdate -color white /i2c_slave_tb/reg_read_addr
add wave -noupdate -divider {Slave Internal State}
add wave -noupdate -color magenta /i2c_slave_tb/dut/current_state
add wave -noupdate -color magenta /i2c_slave_tb/dut/next_state
add wave -noupdate -color magenta /i2c_slave_tb/dut/shift_reg
add wave -noupdate -color magenta /i2c_slave_tb/dut/bit_counter
add wave -noupdate -color magenta /i2c_slave_tb/dut/address_matched
add wave -noupdate -color magenta /i2c_slave_tb/dut/read_mode
add wave -noupdate -color magenta /i2c_slave_tb/dut/current_reg_addr
add wave -noupdate -color magenta /i2c_slave_tb/dut/sda_out
add wave -noupdate -color magenta /i2c_slave_tb/dut/sda_oe
add wave -noupdate -divider Synchronization
add wave -noupdate -color orange /i2c_slave_tb/dut/sda_sync
add wave -noupdate -color orange /i2c_slave_tb/dut/scl_sync
add wave -noupdate -color orange /i2c_slave_tb/dut/sda_falling
add wave -noupdate -color orange /i2c_slave_tb/dut/scl_falling
add wave -noupdate -color orange /i2c_slave_tb/dut/scl_rising
add wave -noupdate /i2c_slave_tb/dut/register_file
add wave -noupdate -divider {Test Status}
add wave -noupdate -color green /i2c_slave_tb/test_pass
add wave -noupdate -color red /i2c_slave_tb/test_fail
add wave -noupdate -color blue /i2c_slave_tb/test_counter
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {30000000 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 200
configure wave -valuecolwidth 80
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 5000
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {407641 ps} {3249595 ps}
