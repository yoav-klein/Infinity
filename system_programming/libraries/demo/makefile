
gd=gcc -ansi -pedantic-errors -Wall -Wextra -g

.PHONY: clean

all: foo bar
	$(gd) main.c -L. -lfoo -lbar -Wl,-rpath=.

foo: foo.c 
	$(gd) -c -fpic foo.c
	$(gd) -shared -o libfoo.so foo.o

bar: bar.c
	$(gd) -c -fpic bar.c
	$(gd) -shared -o libbar.so bar.o

clean:
	rm *.so *.o *.out

