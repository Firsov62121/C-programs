all:
	gcc -o h5 -std=c11 -Wall -ggdb3 main.c -L. -lmy_string
valgrind:
	valgrind --leak-check=full --track-origins=yes ./h5
final:
	gcc main.c -L. -lmy_string -o homework5
startUsual:
	./h5
startFinal:
	./homework5