CC = gcc
CFLAGS = -g -Wall -O3 -lncurses
TARGET=chess

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o build/$(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
