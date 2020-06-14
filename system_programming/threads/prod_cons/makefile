srcdir = ./src/
objdir = ./objs/
libdir = /home/ol69/git/ds/lib/
bindir = ./bin/
hdir = /home/ol69/git/ds/include/

CC=gcc
CDFLAG=-ansi -pedantic-errors -Wall -Wextra -g
CRFLAG=-ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3
SRCS = $(wildcard $(srcdir)*.c)
OBJS = $(patsubst $(srcdir)%.c, $(objdir)%.o ,$(SRCS)) 
LIBD = $(libdir)libds_debug.so
BINS = $(patsubst $(srcdir)%.c, $(bindir)%.out ,$(SRCS))

build: $(BINS) 

$(bindir)%.out: $(objdir)%.o $(LIBD)
	$(CC) $(CDFLAG) $^ -o $@ -lpthread -lm

$(objdir)%.o: $(srcdir)%.c
	$(CC) $(CDFLAG) -I$(hdir) $^ -c -o $@

	
clean:
	rm bin/*
	rm objs/*	
