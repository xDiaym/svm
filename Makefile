TARGET = svm
SRCS = main.c
OBJS = $(SRCS:.c=.o)
INCLUDE_DIR = include
CFLAGS = -Wall -Wextra -I$(INCLUDE_DIR)
LDFLAGS =

.PHONY: all clean

all: $(TARGET)

clean:
	rm -rf $(TARGET) *.o

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
