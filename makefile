CC=g++
CFLAGS= -std=c++11 -pthread
FILES= WaTor.cpp 
OUT_EXE=WaTor

build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES) $(CFLAGS)

clean:
	rm *.o

sfmlrun: main.o
	g++ -c main.cpp
	g++ main.o -o main -lsfml-graphics -lsfml-window -lsfml-system
	./main

sfmlcompile: 
	g++ -c main.cpp
