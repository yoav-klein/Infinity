incdir=include/
master_inc=$(incdir)master/
slave_inc=$(incdir)slave/

srcdir=srcs/
master_srcs=$(srcdir)master/
slave_srcs=$(srcdir)slave/

objdir=objs/
master_objs=$(objdir)master/
slave_objs=$(objdir)slave/

libdir=lib/

ddir=dfiles/
master_ddir=$(ddir)master/
slave_ddir=$(ddir)slave/

master_dir=master/
slave_dir=slave/
rpi_dir=rpi/

SRCS = $(wildcard $(srcdir)*.cpp)
MASTER_SRCS= $(wildcard $(master_srcs)*.cpp)
SLAVE_SRCS= $(wildcard $(slave_srcs)*.cpp)

OBJS = $(patsubst $(srcdir)%.cpp, $(objdir)%.o, $(SRCS))
OBJS_PI = $(patsubst $(srcdir)%.cpp, $(objdir)%_pi.o, $(SRCS))
MASTER_OBJS = $(patsubst $(master_srcs)%.cpp, $(master_objs)%.o, $(MASTER_SRCS))
SLAVE_OBJS = $(patsubst $(slave_srcs)%.cpp, $(slave_objs)%.o, $(SLAVE_SRCS))
SLAVE_OBJS_PI = $(patsubst $(slave_srcs)%.cpp, $(slave_objs)%_pi.o, $(SLAVE_SRCS))

DEPS = $(patsubst $(srcdir)%.cpp, $(ddir)%.d ,$(SRCS))
MASTER_DEPS = $(patsubst $(master_srcs)%.cpp, $(master_ddir)%.d ,$(MASTER_SRCS))
SLAVE_DEPS = $(patsubst $(slave_srcs)%.cpp, $(slave_ddir)%.d, $(SLAVE_SRCS))
CPP=g++
CPPCROSS =/rpi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++
MASTER = master.cpp
SLAVE = slave.cpp
GLOBLIB = libglob.so
LDFLAGS = -L$(slave_dir) -L$(master_dir) -lglob -ldl -pthread -lconfig++ -Wl,-rpath=. \
-Llib 
LDFLAGS_PI = -L$(rpi_dir) -lglob -ldl -pthread -lconfig++ -Wl,-rpath=.

LIBDIRS=$(sort $(dir $(wildcard $(libdir)*/)))

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS = -ansi -pedantic-errors -Wall -Wextra -g -std=c++11
else
	CFLAGS = -ansi -pedantic-errors -Wall -Wextra -std=c++11 -DNDEBUG
endif

.PHONY: master slave all rpi 

all: master slave rpi

master: make_glob_lib $(master_dir)master.out

slave: make_glob_lib $(slave_dir)slave.out

rpi: make_libconfig make_glob_lib $(rpi_dir)slave.out

$(master_dir)master.out: $(MASTER) $(OBJS) $(MASTER_OBJS)
	$(CPP) $(CFLAGS) -I$(incdir) -I$(master_inc) -o $@ $^ $(LDFLAGS)

$(slave_dir)slave.out: $(SLAVE) $(OBJS) $(SLAVE_OBJS)
	$(CPP) $(CFLAGS) -I$(incdir) -I$(slave_inc) -o $@ $^ $(LDFLAGS)

$(rpi_dir)slave.out:  $(SLAVE) $(OBJS_PI) $(SLAVE_OBJS_PI)
	$(CPPCROSS) $(CFLAGS) -I$(incdir) -I$(slave_inc) -o $@ $^ $(LDFLAGS_PI)

.PHONY: make_glob_lib make_libconfig
make_glob_lib: 
	make -f $(libdir)globals/makefile

make_libconfig:
	make -f $(libdir)libconfig/makefile

-include $(DEPS)
-include $(MASTER_DEPS)
-include $(SLAVE_DEPS)

$(ddir)%.d: $(srcdir)%.cpp
	$(CPP) $(CFLAGS) -I$(incdir) -I$(master_inc) -I$(slave_inc)\
	 -MM -MT $(patsubst $(ddir)%.d, $(objdir)%.o, $@) $< > $@
	echo "\t"$(CPP) $(CFLAGS) -c -fpic -I$(incdir) -I$(master_inc) -I$(slave_inc)\
	 -o $(patsubst $(ddir)%.d, $(objdir)%.o ,$@) $< >> $@
	$(CPPCROSS) $(CFLAGS) -I$(incdir) -I$(master_inc) -I$(slave_inc)\
	 -MM -MT $(patsubst $(ddir)%.d, $(objdir)%_pi.o, $@) $< >> $@
	echo "\t"$(CPPCROSS) $(CFLAGS) -c -fpic -I$(incdir) -I$(master_inc) -I$(slave_inc)\
	 -o $(patsubst $(ddir)%.d, $(objdir)%_pi.o ,$@) -DRPI $< >> $@ 

.PHONY: clean

clean:
	rm   $(objdir)*.o $(master_objs)*.o $(slave_objs)*.o $(BIN) $(ddir)*.d $(master_ddir)*.d $(slave_ddir)*.d $(master_dir)*.out $(slave_dir)*.out $(slave_dir)logfile.txt $(master_dir)logfile.txt $(rpi_dir)slave.out
	
clean_glob:
	make -f $(libdir)globals/makefile clean 

clean_config:	
	make -f $(libdir)libconfig/makefile clean
	
	
