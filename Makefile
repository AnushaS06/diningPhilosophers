all: dine
dine: dine.o
	gcc -pthread -o dine dine.o
dine.o: dine.c
	gcc -Wall -g -c dine.c
clean:
	rm -f *.o
	rm -f dine
	
