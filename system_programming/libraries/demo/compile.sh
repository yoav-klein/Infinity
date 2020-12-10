

function clean()
{
	rm *.so *.o *.out
}

function build_foo()
{
	gd -c -fpic foo.c
	gd -shared -o libfoo.so foo.o
}

function build_bar()
{
	gd -c -fpic bar.c
	gd -shared -o libbar.so bar.o
}
