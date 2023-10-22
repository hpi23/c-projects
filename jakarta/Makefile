FILE:=output.c

build:
	gcc -c -ggdb -Wall -Werror -fpic ./runtime/$(FILE) -o ./runtime/$(FILE).o
	gcc -shared -o ./runtime/$(FILE).so ./runtime/$(FILE).o

run:
	rm -rf runtime
	mkdir -p runtime
	gcc -ggdb main.c ../dynstring/dynstring.c ../list/list.c -o main
	LD_LIBRARY_PATH=/home/mik/Coding/hpi/jakarta/runtime ./main
