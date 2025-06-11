echo "Running tests..."
gcc -g cpu/cpu.c cpu/addressing_modes.c cpu/instructions.c cpu/cpu_test.c bus/bus.c ppu/ppu.c cartridge/cartridge.c -o test-roms/test.bin
test-roms/test.bin
echo "Testing finished"

# Tests are ignored for now since I will be focusing on graphical capabilities of the NES
