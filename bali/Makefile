FILE:=output.c

build:
	gcc -c -ggdb -Wall -Werror -fpic ./runtime/$(FILE) -o ./runtime/$(FILE).o
	gcc -ggdb -shared -o ./runtime/$(FILE).so ./runtime/$(FILE).o

run:
	rm -rf runtime
	mkdir -p runtime
	gcc -ggdb main.c ../dynstring/dynstring.c ../list/list.c ./MyClass.c ./MyClass_dispatcher.c -o main
	# LD_LIBRARY_PATH=$(PWD)/runtime ./main
	LD_LIBRARY_PATH=/home/mik/Coding/hpi/hpi-c-tests/bali/runtime ./main
