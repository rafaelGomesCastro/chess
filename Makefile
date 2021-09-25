CC = gcc
CFLAGS = -g -Wall -O3 -lncurses
DIR=build
OBJFILES = $(DIR)/movements.o $(DIR)/$(TARGET).o
TARGET=chess

all:
	gcc -Wall -O0 -g -o $(TARGET) *.c

# all: $(TARGET)
#
# $(TARGET): $(OBJFILES)
# 	$(CC) $(CFLAGS) -o
#
# clean:
# 	$(RM) $(TARGET)
