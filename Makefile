# Compiler
CC = gcc
# -Wall -Wextra: all warnings, C99, debug info
CFLAGS = -Wall -Wextra -std=c99 -g
# math library (difftime)
LDFLAGS = -lm

TARGET = ParkManSys
SRCDIR = src
SRCS = $(SRCDIR)/main.c \
       $(SRCDIR)/menu.c \
       $(SRCDIR)/car.c \
       $(SRCDIR)/fee.c \
       $(SRCDIR)/month_card.c \
       $(SRCDIR)/park_record.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)
	@echo "Build OK -> ./$(TARGET)"

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(TARGET) $(OBJS) *.dat
	@echo "Cleaned build artifacts and runtime data files"

.PHONY: all clean
