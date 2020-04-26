all: compile

compile: jdv.o p4.o prox.o tron.o
	gcc -Wall jdv.o -o jdv
	gcc -Wall p4.o -o p4
	gcc -Wall prox.o -o prox
	gcc -Wall tron.o -o tron

jdv.o: jdv.c
	gcc -Wall jdv.c -c

p4.o: p4.c
	gcc -Wall p4.c -c

prox.o: prox.c
	gcc -Wall prox.c -c

tron.o: tron.c
	gcc -Wall tron.c -c

clean:
	rm *.o exec