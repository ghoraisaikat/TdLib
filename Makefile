CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

TARGET = threads
SRC = threads.c src/uthread.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
