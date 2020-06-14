
all: waiter.out quitter.out
	
%.out: %.c
	gcc -o $@ $^
	
.PHONY: clean

clean:
	rm *.out 
