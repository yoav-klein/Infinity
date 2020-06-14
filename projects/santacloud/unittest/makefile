proj_dir=$(HOME)/git/projects/santacloud/

incdir=$(proj_dir)include/
master_inc=$(incdir)master/
slave_inc=$(incdir)slave/

srcdir=$(proj_dir)srcs/
master_srcs=$(srcdir)master/
slave_srcs=$(srcdir)slave/

objdir=$(proj_dir)objs/
master_objs=$(objdir)master/
slave_objs=$(objdir)slave/

libdir=$(proj_dir)lib/

ddir=$(proj_dir)dfiles/
master_ddir=$(ddir)master/
slave_ddir=$(ddir)slave/

SRCS = $(wildcard $(srcdir)*.cpp)
MASTER_SRCS= $(wildcard $(master_srcs)*.cpp)
SLAVE_SRCS= $(wildcard $(slave_srcs)*.cpp)

OBJS = $(patsubst $(srcdir)%.cpp, $(objdir)%.o, $(SRCS))
MASTER_OBJS = $(patsubst $(master_srcs)%.cpp, $(master_objs)%.o, $(MASTER_SRCS))
SLAVE_OBJS = $(patsubst $(slave_srcs)%.cpp, $(slave_objs)%.o, $(SLAVE_SRCS))

DEPS = $(patsubst $(srcdir)%.cpp, $(ddir)%.d ,$(SRCS))
MASTER_DEPS = $(patsubst $(master_srcs)%.cpp, $(master_ddir)%.d ,$(MASTER_SRCS))
SLAVE_DEPS = $(patsubst $(slave_srcs)%.cpp, $(slave_ddir)%.d, $(SLAVE_SRCS))
CPP = g++
MASTER = master.cpp
SLAVE = slave.cpp
GLOBLIB=$(libdir)libglob.so
TESTS = $(wildcard *.cpp)
BINS = $(patsubst %.cpp, %.out, $(TESTS)) 

LIBDIRS=$(sort $(dir $(wildcard $(libdir)*/)))

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g -std=c++11
else
	CFLAGS = -ansi -pedantic-errors -Wall -Wextra -std=c++11 -DNDEBUG
endif

all:  $(BINS)

%_test.out: %_test.cpp $(OBJS) $(MASTER_OBJS) $(SLAVE_OBJS) $(GLOBLIB)
	$(CPP) $(CFLAGS) -I$(incdir) -I$(master_inc) -I$(slave_inc)  -o $@ $^	-pthread -ldl

.PHONY: $(GLOBLIB)
$(GLOBLIB): 
	make -f $(libdir)globals/makefile
	
make_libs: 
	for dir in $(LIBDIRS); do make -f $$dir/makefile; done 

-include $(DEPS)
-include $(MASTER_DEPS)
-include $(SLAVE_DEPS)

$(ddir)%.d: $(srcdir)%.cpp
	$(CPP) $(CFLAGS) -I$(incdir) -I$(master_inc) -I$(slave_inc)\
	 -MM -MT $(patsubst $(ddir)%.d, $(objdir)%.o, $@) $< > $@
	echo "\t"$(CPP) $(CFLAGS) -c -fpic -I$(incdir) -I$(master_inc) -I$(slave_inc)\
	 -o $(patsubst $(ddir)%.d, $(objdir)%.o ,$@) $< >> $@

.PHONY: clean

clean:
	rm  ../*.out *.out $(objdir)*.o $(master_objs)*.o $(slave_objs)*.o $(BIN) $(ddir)*.d $(master_ddir)*.d $(slave_ddir)*.d
	
