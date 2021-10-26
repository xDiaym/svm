TARGET=svm
SRCS=$(shell find . -name "*.c")
OBJS=$(SRCS:.c=.o)
INCLUDE_DIR=include
CFLAGS=-std=c11 -ggdb -g -Wall -Wextra -I$(INCLUDE_DIR)
LDFLAGS=-lm

.PHONY: all clean lint format

all: $(TARGET)

clean:
	find . -name '*.o' -exec rm {} \;
	rm $(TARGET)

lint:
	@eval 'scripts/lint $(CFLAGS)'

format:
	@scripts/format

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
