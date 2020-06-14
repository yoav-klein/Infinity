projdir=$(HOME)/git/projects/santacloud/
incdir=$(HOME)/git/projects/santacloud/include/
master_inc=$(incdir)master/
slave_inc=$(incdir)slave/

srcdir=$(projdir)srcs/
libdir=$(projdir)lib/
currdir=$(libdir)globals/

master_dir=$(projdir)master/
slave_dir=$(projdir)slave/
rpi_dir=$(projdir)rpi/

SRCS=$(wildcard $(currdir)*.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
OBJS_PI=$(patsubst %.cpp, %_pi.o, $(SRCS))
DEPS=$(patsubst %.cpp, %.d, $(SRCS))
TARGETS=$(master_dir)libglob.so $(slave_dir)libglob.so $(rpi_dir)libglob.so
CPP=g++
CPPCROSS =/rpi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++

CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g -std=c++11
	
all: $(TARGETS)

$(rpi_dir)libglob.so: $(OBJS_PI)
	echo HERE
	$(CPPCROSS) $(CFLAGS) -shared -o $@ $^

%libglob.so: $(OBJS)
	$(CPP) $(CFLAGS) -shared -o $@ $^ 

-include $(DEPS)

$(currdir)%.d: $(currdir)%.cpp
	$(CPP) $(CFLAGS) -I$(incdir) -I$(master_inc) -I$(slave_inc) -MM -MT $(patsubst %.d, %.o, $@) $< > $@
	echo "\t"$(CPP) $(CFLAGS) -c -I$(incdir) -I$(master_inc) -I$(slave_inc)\
	 -fpic -o $(patsubst %.d, %.o, $@)  $< >> $@
	$(CPPCROSS) $(CFLAGS) -I$(incdir) -I$(master_inc) -I$(slave_inc) -MM -MT $(patsubst %.d, %_pi.o, $@) $< >> $@
	echo "\t"$(CPPCROSS) $(CFLAGS) -c -I$(incdir) -I$(master_inc) -I$(slave_inc)\
	 -fpic -o $(patsubst %.d, %_pi.o, $@)  $< >> $@
	

.PHONY: clean

clean:
	rm $(currdir)*.d $(currdir)*.o $(TARGETS)
