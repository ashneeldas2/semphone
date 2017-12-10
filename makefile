all: control.c main.c
	gcc -o control control.c
	gcc -o main main.c
clean: all
	rm control main
