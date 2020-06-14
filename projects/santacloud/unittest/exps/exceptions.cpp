#include <iostream> // 
#include <cstdlib>

struct nbd_error : public std::runtime_error
{
	nbd_error(const std::string& s = "nbd_error")
		: std::runtime_error(s) { }
};

void Throw()
{
	throw nbd_error("No");
}

