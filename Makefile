TARGET=svm
SRCS=$(shell find . -name '*.c')
OBJS=$(SRCS:.c=.o)
INCLUDE_DIR=include
CFLAGS=-std=c99 -ggdb -g -Wall -Wextra -I$(INCLUDE_DIR)
LDFLAGS=

.PHONY: all clean lint

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
