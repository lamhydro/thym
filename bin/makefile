# Define the compiler and the flags
CC = gcc
CFLAGS = -I../inc -Wall -g
LDFLAGS = -lm  # Link math library

# Define the source and header files
SRCDIR = ../src
INCDIR = ../inc
BINDIR = .
BINDIR2 = bin
LINK_NAME = ../thym

# Define other varible
MODEL = 1

# List of source files
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Generate object files from source files
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BINDIR)/%.o)

# Name of the output binary
TARGET = $(BINDIR)/thym

# Rule for building the program
all: $(TARGET) symlink

# Rule for compiling the program
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Rule for compiling the source files into object files
$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -DMODEL=$(MODEL) -c $< -o $@
	
# Create symbolic link
symlink:
	@rm -f $(LINK_NAME)  # Remove the existing link if it exists
	@ln -s $(BINDIR2)/thym $(LINK_NAME)

# Rule for cleaning up object files and the binary
clean:
	rm -f $(BINDIR)/*.o $(TARGET)

# Phony targets
.PHONY: all clean
CC=gcc

