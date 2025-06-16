CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -Llib -lraylib -lGL -lm -lpthread -ldl -lrt
TARGET = main.bin

default:
	$(CC) main.c $(CFLAGS) $(LDFLAGS) -o $(TARGET)
	./$(TARGET)

test:
	$(CC) test/test.c $(CFLAGS) $(LDFLAGS) -o $(TARGET)
	./$(TARGET)
