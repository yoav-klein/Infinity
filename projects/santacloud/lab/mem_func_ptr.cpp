#include <iostream>


class Dispatcher
{
public:
	Dispatcher(int a)
		: m_a(a) { }
	
	void show() { std::cout << m_a << std::endl; }

private:
	int m_a;
};

class Displayer
{
public:
	using Function = void (Dispatcher::*)();
	
	Displayer(Dispatcher& dispatcher, Function show_func)
		: m_dispatcher(dispatcher), m_show_func(show_func)
	{
		
	}
	
	void display()
	{
		(m_dispatcher.*m_show_func)();
	}
private:
	Dispatcher& m_dispatcher;
	Function m_show_func;

};

int main()
{
	Dispatcher disp(22);
	
	Displayer displayer(disp, &Dispatcher::show);
	
	displayer.display();
	
	return 0;
}	
