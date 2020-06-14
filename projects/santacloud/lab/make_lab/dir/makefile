
CC=gcc
SRCS=$(wildcard *.c)
DEPS=$(patsubst %.c, %.d, $(SRCS))


all:
	echo g
	
-include $(DEPS)

%.d: %.c
	$(CC) -MM -MT $(patsubst %.d, objs/%.o, $@) $< > $@
	echo "\t"$(CC) -c -fpic -o $ @ >> $@
