TARGET=svm
SRCS=main.c src/objects/object.c src/objects/string_object.c src/objects/int_object.c src/panic.c
OBJS=$(SRCS:.c=.o)
INCLUDE_DIR=include
CFLAGS=-ggdb -g -Wall -Wextra -std=c11 -I$(INCLUDE_DIR)
LDFLAGS=

.PHONY: all clean

all: $(TARGET)

clean:
	find . -name '*.o' -exec rm {} \;
	rm $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
