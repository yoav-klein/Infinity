

currdir=$(HOME)/git/projects/santacloud/lib/test_tasks/
LIBS=$(sort $(dir $(wildcard $(currdir)*/)))

DIRS=$(shell ls $(currdir))

all:
	for dir in $(DIRS); do make -f $$dir/makefile; done

clean:
	for dir in $(DIRS); do make -f $$dir/makefile clean; done
