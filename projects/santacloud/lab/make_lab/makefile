
DIRS = $(sort $(dir $(wildcard */)))



all:
	for dir in $(DIRS); do make -f $$dir/makefile; done

compile: $(DIRS)
	make -f $<makefile
	

#file:
#	touch file
	
dire: dire/a.c
	echo gg

