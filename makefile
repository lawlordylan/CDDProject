CC=g++
CFLAGS= -std=c++11 -pthread
FILES= WaTor.cpp 
OUT_EXE=WaTor

build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES) $(CFLAGS)

clean:
	rm *.o
