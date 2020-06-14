
dsdir=/home/yoav/git/ds/
incdir=$(dsdir)include/
srcdir=$(dsdir)src/
libdir =$(dsdir)lib/
LIBD=$(libdir)libds_debug.so

CC=gcc

sort.out: shuffle_sort.c 
	$(CC) -o $@ $^ -I$(incdir) $(LIBD) -lm -lpthread
