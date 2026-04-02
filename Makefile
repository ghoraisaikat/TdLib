CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

TARGET = build/threads
SRC = threads.c src/uthread.c

UCREATION = test/creation_overhead_u.c src/uthread.c
UBLOCKING = test/blocking_u.c src/uthread.c
USWITCHING = test/context_switching_u.c src/uthread.c

TCREATION = test/build/creation_u
TBLOCKING = test/build/blocking_u
TSWITCHING = test/build/switching_u

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

creation:
	$(CC) $(CFLAGS) $(UCREATION) -o $(TCREATION)

blocking:
	$(CC) $(CFLAGS) $(UBLOCKING) -o $(TBLOCKING)

switching:
	$(CC) $(CFLAGS) $(USWITCHING) -o $(TSWITCHING)


tests: creation blocking switching
	@true


run: all
	./$(TARGET)

clean_test:
	rm -f $(TCREATION) $(TBLOCKING) $(TSWITCHING)

clean_target:
	rm -f $(TARGET)

clean: clean_test clean_target
	@true
