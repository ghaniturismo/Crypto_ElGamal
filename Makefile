CC = gcc
CFLAGS = -lgmpxx -lgmp

OBJS = main.c euclide.c

main: $(OBJS)
	$(CC) -o main $(OBJS) $(CFLAGS) 

main.o: main.c euclide.c


prime_genere: prime_genere.c
	$(CC) -o prime_genere prime_genere.c $(CFLAGS) 

prime_genere.o: prime_genere.c

euclide: euclide.c
	$(CC) -o euclide euclide.c $(CFLAGS) 

euclide.o: euclide.c


clean:
	rm -f *~ *.o main prime_genere euclide
