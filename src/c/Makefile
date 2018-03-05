all: rpg

rng: rpg.c rngs.o
	gcc -o rpg rpg.c rngs.o

rngs.o: rngs.c
	gcc -c rngs.c -o rngs.o

clean:
	rm *.o
