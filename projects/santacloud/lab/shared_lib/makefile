
CPP=g++
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -std=c++11


all: libbar.so libfoo.so

lib%.so: %.o
	$(CPP) $(CFLAGS) -shared -o $@ $^
	
%.o: %.cpp
	$(CPP) $(CFLAGS) -c -fpic $^

.PHONY: clean

clean:
	rm *.out *.o *.so
