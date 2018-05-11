CC=g++
CFLAGS= -std=c++11 -pthread
FILES= WaTor.cpp 
OUT_EXE=WaTor

build:
	g++ -std=c++11 -pthread -c WaTor.cpp -fopenmp
	g++ -std=c++11 -pthread WaTor.o -o WaTor -lsfml-graphics -lsfml-window -lsfml-system -fopenmp
	./WaTor

clean:
	rm *.o

sfmlrun: main.o
	g++ -c main.cpp
	g++ main.o -o main -lsfml-graphics -lsfml-window -lsfml-system
	./main

sfmlcompile: 
	g++ -c main.cpp
