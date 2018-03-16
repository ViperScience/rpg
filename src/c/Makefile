all: rpg

rpg: rpg.c rngs.o rngs.h
	gcc -o rpg rpg.c rngs.o

rngs.o: rngs.c rngs.h
	gcc -c rngs.c -o rngs.o

clean:
	rm *.o rpg
