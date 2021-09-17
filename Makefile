TARGET = svm
SRCS = main.c src/object.c src/string_object.c
OBJS = $(SRCS:.c=.o)
INCLUDE_DIR = include
CFLAGS = -Wall -Wextra -std=c11 -I$(INCLUDE_DIR)
LDFLAGS =

.PHONY: all clean

all: $(TARGET)

clean:
	rm -rf $(TARGET) *.o

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
