hepsi: derle calistir

derle:
	gcc -I ./include/ -o ./lib/Shell.o -c ./src/Shell.c
	gcc -I ./include/ -o ./bin/Test ./lib/Shell.o ./src/Test.c

calistir:
	./bin/Test