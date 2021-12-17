all:
	gcc -o h2 -std=c11 -Wall -ggdb3 main.c point_vector.c matrix.c
valgrind:
	valgrind --leak-check=full --track-origins=yes ./h2
final:
	gcc main.c point_vector.c matrix.c -o homework2
startUsual:
	./h2
startFinal:
	./homework2