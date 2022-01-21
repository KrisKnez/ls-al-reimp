CC = gcc
CFLAGS = -Wall

TARGETS = vjezba7

default: vjezba7

all: $(TARGETS)

vjezba7: vjezba7.o
	$(CC) $(CFLAGS) vjezba7.o -o vjezba7

clean:
	rm -f *.o *~ a.out $(TARGETS)
	
.c.o:
	$(CC) $(CFLAGS) -c $<		
