# ModelSim compile script for I2C Slave without vopt

# Clean previous compilation
vdel -all -lib work

# Create library
vlib work
vmap work work

echo "========================================="
echo "Compiling I2C Slave design..."
echo "========================================="

# Compile Verilog files with full debugging
vlog -sv +acc +cover +incdir i2c_slave.sv
vlog -sv +acc +cover i2c_slave_tb.sv

echo "========================================="
echo "I2C Slave compilation completed successfully!"
echo "========================================="