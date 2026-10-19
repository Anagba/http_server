# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Target Executable Name
TARGET = server

# All source files and their corresponding object files
SRCS = src/main.c src/server.c src/client_handler.c src/mime.c src/rate_limiter.c src/socket_utils.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link all object files together to create the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Generic pattern rule to compile any .c file in src/ into a .o file
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean
