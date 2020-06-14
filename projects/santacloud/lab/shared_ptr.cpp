// 26.12 - i want to: have a function that simulates Kamikaze::send_request.
// in this function i receive a unique_ptr, and i want to conver it to 
// shared_ptr, push it to a map, and if it's a write request, push it 
// again with a different key. (another function). i want to end this process
// with a reference count of 2.

#include <iostream>
#include <memory>

void send_request(std::unique_ptr<int> ptr);
void send_regular(std::shared_ptr<int> ptr);
void send_backup(std::shared_ptr<int> ptr);


void send_request(std::unique_ptr<int> ptr)
{
	std::cout << "send_Request" << std::endl;
	std::shared_ptr<int> sptr = std::move(ptr);
	
	send_regular(sptr);
	send_backup(sptr);
	
	std::cout << "send_request: " << sptr.use_count() << std::endl;
}

void send_regular(std::shared_ptr<int> ptr)
{
	std::cout << "send_regular: " << ptr.use_count() << std::endl;
	// push to map
}

void send_backup(std::shared_ptr<int> ptr)
{
	std::cout << "send_backup: " << ptr.use_count() << std::endl;
	// push to map
}


// here i want to try moving from shared_ptr to unique_ptr
// YOU CAN'T!!
std::unique_ptr<int> receive_reply()
{
	std::shared_ptr<int> sp(new int(224));
	std::unique_ptr<int> uptr(sp);
	
	return ptr;
}


int main()
{
	std::cout << *receive_reply();

	return 0;
}
