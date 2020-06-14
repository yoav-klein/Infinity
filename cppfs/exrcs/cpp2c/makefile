CPP=g++

CPPFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -std=c++98

CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g

all : c.out cpp.out
	./c.out > output_c.txt
	./cpp.out > output_cpp.txt
	diff output_c.txt output_cpp.txt

c.out : cpp2c.c
	$(CC) $(CFLAGS) -o $@ $^ 	

cpp.out : cpp2csrc.cpp
	$(CPP) $(CPPFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm *.txt *.out
	
