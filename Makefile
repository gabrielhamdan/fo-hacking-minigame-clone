all:
	gcc -o bin main.c

run:
	./bin

debug:
	./bin -d