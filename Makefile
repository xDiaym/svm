TARGET=svm
SRCS=main.c \
	src/objects/object.c \
	src/objects/string_object.c \
	src/objects/builtin_function_object.c \
	src/objects/int_object.c \
	src/objects/null_object.c \
	src/panic.c \
	src/allocator.c
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

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
