EXECUTABLE = UOL

CC = gcc

CFLAGS = -Wall -g -lusb-1.0 -I./include

OBJECTS = UOL.o

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

UOL.o: UOL.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
