CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c11
TARGET=metrics-agent

all: $(TARGET)
$(TARGET): src/main.c
	$(CC) $(CFLAGS) -o $@ $<
clean:
	rm -f $(TARGET)
.PHONY: all clean
