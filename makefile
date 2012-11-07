all: main

main: main.o genzipf.o
	g++ main.o genzipf.o -o main

genzipf.o: genzipf.c
	gcc -c genzipf.c

main.o: main.cpp
	g++ -c main.cpp
	
clean:
	rm main.o genzipf.o main