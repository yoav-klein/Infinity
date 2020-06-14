srcdir=./lib_srcs/
objdir=./lib_objs/
incdir=$(HOME)/git/ds/include/

CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g
LIB_SRCS=$(wildcard $(srcdir)*.c)
OBJS=$(patsubst $(srcdir)%.c, $(objdir)%.o, $(LIB_SRCS))
SH_LIB=libwd_shared.so
STAT_LIB=wd_api.a
WD_EXE=wd_exec.out
TEST=wd_test.out

.PHONY: all

all: $(TEST) $(WD_EXE)
	
$(TEST): test.c $(STAT_LIB) $(SH_LIB) 
	$(CC) $(CFLAGS) -o $@ $^ -Wl,-rpath=$(PWD) -lpthread

$(WD_EXE): watchdog.c $(SH_LIB)
	$(CC) $(CFLAGS)  -I$(incdir) -o $@ $^ -Wl,-rpath=$(PWD)  

$(STAT_LIB): appside.c
	$(CC) $(CFLAGS) -I$(incdir) -c -o appside.o $^
	ar rcs $@ appside.o

$(SH_LIB): $(OBJS)
	$(CC) -shared -o $@ $^

$(objdir)%.o: $(srcdir)%.c
	$(CC) $(CFLAGS) -c -I$(incdir) -fpic -o $@ $^ -lm -D_POSIX_C_SOURCE

.PHONY: clean

clean: 
	rm $(objdir)*.o $(SH_LIB) *.out *.o *.a
