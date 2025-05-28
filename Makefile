CC=gcc
CFLAGS=-Wall -Iinclude -g
BINARY=icsh
SRC = ./icsh.c ./welcome_banner.c ./builtins.c ./command_processor.c ./external.c ./signal_handler.c ./file_processor.c ./io_redirect.c
OBJ = $(SRC:.c=.o)

all: icsh

icsh: $(OBJ)
	$(CC) -o $(BINARY) $(CFLAGS) $^
	rm -f $(OBJ)

.PHONY: clean

clean:
	rm -f $(BINARY)

# Compile each .c file into a .o file
./%.o: ./%.c
	$(CC) $(CFLAGS) -c $< -o $@
