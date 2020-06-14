dsdir=/home/yoav/git/ds/
incdir=$(dsdir)include/
srcdir=$(dsdir)src/
libdir =$(dsdir)lib/
LIBD=$(libdir)libds_debug.so

CC=gcc

.PHONY: watchdog

all: test.out

test.out: watchdog.c
	$(CC) -o test.out -I$(incdir) $^ $(LIBD) -lm -lpthread

.PHONY: clean
clean: 
	rm  *.out
	
