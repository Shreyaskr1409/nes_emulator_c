CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
CFLAGS_ALT = -Iinclude
LDFLAGS = -Llib -lraylib -lGL -lm -lpthread -ldl -lrt
TARGET = main.bin
CFILES = cpu/cpu.c cpu/addressing_modes.c cpu/instructions.c \
		 bus/bus.c \
		 cartridge/cartridge.c cartridge/mapper.c cartridge/mapper000.c \
		 ppu/ppu.c

default:
	$(CC) main.c $(CFLAGS) $(LDFLAGS) -o $(TARGET)
	./$(TARGET)

test:
	$(CC) test/test.c $(CFILES) $(CFLAGS_ALT) $(LDFLAGS) -o $(TARGET)
	./$(TARGET)
