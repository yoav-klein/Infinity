projdir=$(HOME)/git/projects/santacloud/
incdir=$(HOME)/git/projects/santacloud/include/
srcdir=$(projdir)srcs/
libdir=$(projdir)lib/
objdir=$(projdir)objs/
currdir=$(PWD)/

SRCS=$(wildcard $(currdir)*.cpp)
PROJOBJS=$(wildcard $(objdir)*.o)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
DEPS=$(patsubst %.cpp, %.d, $(SRCS))
LIB=$(libdir)libcat.so
LIBGLOB=$(libdir)libglob.so
CPP=g++
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -std=c++11


$(LIB): $(OBJS) $(PROJOBJS) $(LIBGLOB)
	$(CPP) $(CFLAGS) -shared -o $@ $^ 

-include $(DEPS)

$(currdir)%.d: $(currdir)%.cpp
	$(CPP) $(CFLAGS) -I$(incdir) -MM -MT $(patsubst %.d, %.o, $@) $< > $@
	echo "\t"$(CPP) $(CFLAGS) -c -I$(incdir) -fpic -o $(patsubst %.d, %.o, $@)  $< >> $@

.PHONY: clean

clean:
	rm $(currdir)*.d $(currdir)*.o $(LIB)
