
CPP=g++
FLAGS=-ansi -pedantic-errors -Wall -Wextra -g -std=c++11

test.out: test.cpp animals.cpp libglob.so 
	$(CPP) $(FLAGS) -o $@ -Wl,-rpath=. $^

libglob.so: globals.o
	$(CPP) $(FLAGS) -shared -o $@ $^

globals.o: globals.cpp
	$(CPP) $(FLAGS) -c -fpic $^
	
.PHONY: clean

clean:
	rm *.o *.so *.out

