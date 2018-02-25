CC = gcc
CFLAGS = -lgmpxx -lgmp

OBJS = main.c

main: $(OBJS)
	$(CC) -o main $(OBJS) $(CFLAGS) 

main.o: main.c

clean:
	rm -f *~ *.o main
