
dsdir=/home/yoav/git/ds/
incdir=$(dsdir)include/
srcdir=$(dsdir)src/
libdir =$(dsdir)lib/
LIBD=$(libdir)libds_debug.so

CC=gcc

.PHONY: watchdog

all: watchdog.out test.out

watchdog.out: watchdog.c 
	$(CC) $^ -g -I$(incdir) -o $@ $(LIBD) -lm
	
test.out: wd_test.o appside.o
	$(CC) -o test.out $^ $(LIBD) -lm -lpthread

wd_test.o: wd_test.c
	$(CC) -c $^

appside.o: appside.c
	$(CC) -c -I$(incdir) $^ 
	
.PHONY: clean
clean: 
	rm  *.out
	rm *.o
	
