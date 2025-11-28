# ModelSim compile script for Audio Router system

# Clean previous compilation
vdel -all -lib work

# Create library
vlib work
vmap work work

echo "========================================="
echo "Compiling Audio Router system..."
echo "========================================="

# Compile all design files
vlog -sv +acc +cover i2c_slave.sv
vlog -sv +acc +cover audio_router.sv
vlog -sv +acc +cover top.sv

# Compile testbench (will use whichever .sv file exists)
vlog -sv +acc +cover top_tb.sv

echo "========================================="
echo "Audio Router system compilation completed successfully!"
echo "========================================="