
CPP=g++
FLAGS=-ansi -pedantic-errors -Wall -Wextra -g -std=c++11

all: libdog.so test.out

test.out: test.cpp libglob.so
	$(CPP) $(FLAGS) -o $@ -Wl,-rpath=. $^ -ldl -lpthread

libglob.so: globals.o
	$(CPP) $(FLAGS) -shared -o $@ $^

globals.o: globals.cpp
	$(CPP) $(FLAGS) -c -fpic $^

libdog.so: lib.o libglob.so
	$(CPP) $(FLAGS) -o $@ -shared -Wl,-rpath=. $^ 

lib.o: lib.cpp
	$(CPP) $(FLAGS) -c -fpic $^ 

.PHONY: clean
clean:
	rm *.o *.so *.out
