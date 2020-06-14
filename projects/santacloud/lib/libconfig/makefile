
proj_dir=$(HOME)/git/projects/santacloud/
curr_dir=$(proj_dir)lib/libconfig/
rpi_dir=$(proj_dir)rpi/

LIB=$(rpi_dir)libconfig++.so
CSRCS=$(wildcard $(curr_dir)*.c)
CPPSRCS=$(wildcard $(curr_dir)*.c++)

COBJS=$(patsubst %.c, %.o, $(CSRCS))
CPPOBJS=$(patsubst %.c++, %.opp, $(CPPSRCS))

CPPCROSS=/rpi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++ -ansi -pedantic-errors -Wall -Wextra -g -std=c++11
CCROSS=/rpi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc

$(LIB): $(COBJS) $(CPPOBJS)
	$(CPPCROSS) -shared -o $@ $^

%.o: %.c
	$(CCROSS) -c -fpic $^ -o $@

%.opp: %.c++
	$(CPPCROSS) -c -fpic $^ -o $@
	
.PHONY: clean

clean:
	rm $(curr_dir)*.o $(LIB)
