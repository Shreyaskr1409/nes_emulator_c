CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude -Iinclude/stc
CFLAGS_ALT = -g -Iinclude -Iinclude/stc
LDFLAGS = -Llib -lraylib -lGL -lm -lpthread -ldl -lrt lib/x86_64-linux-gnu/libstc.a
TARGET = main.bin
# CFILES = cpu/cpu.c cpu/addressing_modes.c cpu/instructions.c \
# 		 bus/bus.c \
# 		 cartridge/cartridge.c cartridge/mapper.c cartridge/mapper000.c \
# 		 ppu/ppu.c
CFILES = cpu/addressing_modes.c cpu/instructions.c cpu/cpu.c \
		 cartridge/mapper000.c cartridge/mapper.c cartridge/cartridge.c \
		 ppu/ppu.c \
		 bus/bus.c

default:
	$(CC) main.c $(CFLAGS) $(LDFLAGS) -o $(TARGET)
	./$(TARGET)

test-1:
	$(CC) test/test_1.c $(CFILES) $(CFLAGS_ALT) $(LDFLAGS) -o $(TARGET)
	./$(TARGET)

test-2:
	$(CC) test/test_2.c $(CFILES) $(CFLAGS_ALT) $(LDFLAGS) -o $(TARGET)
	./$(TARGET)
