echo "Running tests..."
gcc -g cpu/cpu.c cpu/addressing_modes.c cpu/instructions.c cpu/cpu_test.c bus/bus.c -o cpu/test.bin
cpu/test.bin
echo "Testing finished"
