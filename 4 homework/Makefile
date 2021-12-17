all:
	gcc -o h4 -std=c11 -Wall -ggdb3 main.c my_heap.c
valgrind:
	valgrind --leak-check=full --track-origins=yes ./h4
final:
	gcc main.c my_heap.c -o homework4
start:
	./homework4