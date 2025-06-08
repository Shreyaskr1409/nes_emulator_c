echo "Running tests..."
gcc cpu/cpu.c cpu/addressing_modes.c cpu/instructions.c cpu/cpu_test.c bus/bus.c -o cpu/test.bin
echo "Testing finished"
