all:
	gcc -o h1 -std=c11 -Wall -ggdb3 main.c arrays.c
valgrind:
	valgrind --leak-check=full --track-origins=yes ./h1
final:
	gcc main.c arrays.c -o homework1
start:
	./homework1