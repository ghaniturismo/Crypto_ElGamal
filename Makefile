CC = gcc
CFLAGS = -lgmpxx -lgmp -lsodium

OBJS = main.c euclide.c expmod.c

main: $(OBJS)
	$(CC) -o main $(OBJS) $(CFLAGS) 

main.o: $(OBJS)

clean:
	rm -f *~ *.o main 
	rm -f test.txt
	rm -f r.txt