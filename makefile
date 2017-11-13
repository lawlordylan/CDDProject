CC=g++
CFLAGS= -std=c++11 -pthread
FILES= WaTor.cpp gameObject.cpp
OUT_EXE=WaTor

build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES) $(CFLAGS) $(DEBUGFLAGS)

debug: DEBUGFLAGS = -ddebug
debug: build

clean:
	rm *.o
