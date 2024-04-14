patcher.o: patcher.s
	as --gdwarf-2 patcher.s -o patcher.o

combined: main.c patcher.o
	gcc -o combined -Wall -ggdb main.c patcher.o

run: combined
	./combined

gdb: combined
	gdb ./combined
