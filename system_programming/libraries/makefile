
LIBAR=libar.a

a.out: $(LIBAR) libso1.so libso2.so main.c
	gcc main.c libar.a libso1.so -ldl
	
libso1.so: so1.o libar.a
	gcc -shared -o libso1.so so1.o libar.a
	
libso2.so: so2.o libar.a
	gcc -shared -o libso2.so so2.o libar.a
	
so1.o: so1.c
	gcc -fpic -c so1.c

so2.o: so2.c
	gcc -fpic -c so2.c

libar.a: a.o a1.o a2.o
	 ar -rc libar.a a.o a1.o a2.o
	
a.o: a.c
	gcc -c a.c
	
a1.o: a1.c
	gcc -c a1.c

a2.o: a2.c
	gcc -c a2.c
	

