
#include <iostream>

#include "callback_dispatcher.hpp"


class Library
{
public:
	Library(Dispatcher<std::string>* dispatcher)
		: m_dispatcher(dispatcher) { }
	
	void notify_book_available(std::string book)
	{
		m_dispatcher->notify_all(book);
	}

private:
	Dispatcher<std::string>* m_dispatcher;
	
};

class Client
{
public:
	Client(const std::string& name, Dispatcher<std::string>* dispatcher)
		: m_name(name), callback(dispatcher, *this, &Client::receive_notify,
								 &Client::receive_out_of_service) { }

	void receive_notify(const std::string& book_name)
	{
		std::cout << m_name << " received that " << book_name << " is available" << std::endl;
	}
	
	void receive_out_of_service()
	{
		std::cout << m_name << " received out of service message" << std::endl;
	}
	
	
private:
	std::string m_name;
	Callback<std::string, Client> callback;
};

int main()
{
	Dispatcher<std::string> *dispatcher = new Dispatcher<std::string>();
	
	Client yoav("Yoav", dispatcher);
	Client eliyahu("Eliyahu", dispatcher);
	Client yozmach("Yozmach", dispatcher);
	
	Library library(dispatcher);
	
	{
		Client temp("Ploni", dispatcher);
		
		library.notify_book_available("Huckelberry fin");
	}
	library.notify_book_available("Meyers");
	
	library.notify_book_available("HarryPotter");
	
	library.notify_book_available("BavaBatra");
	
	delete(dispatcher);
	
	return 0;
}	


