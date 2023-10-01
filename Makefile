main: list.c main.c tree.c
	gcc -Wall -g3 list.c tree.c main.c -o main

run: main
	./main
