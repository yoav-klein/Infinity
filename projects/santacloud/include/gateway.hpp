#ifndef __GATEWAY_HPP__
#define __GATEWAY_HPP__

/**
 * An abstract class that can connect an input fd to the request engine.
 * Can be used to extract a request from an input fd,to allow creation of RETask.
 * 
 * Tempated on Data, that will hold all arguments needed for RETask creation to
 * be passed to the RequestEngine factory.
*/


#include <unordered_map> // std::pair
#include "tools.hpp"
#include "logger.hpp"

namespace ilrd
{

template <typename Key, typename Args>
class GateWay
{
public:
    GateWay(int fd);
    virtual ~GateWay() = 0;

    GateWay(const GateWay& other) = delete;
    GateWay& operator=(const GateWay& other) = delete;
    GateWay(GateWay&& other) = delete; // ?
    GateWay& operator=(GateWay&& other) = delete; // ?
    
    int get_fd();

    virtual std::pair<Key, Args> read() = 0;
protected:
	void LOG(Logger::Severity sever, const std::string& msg, int line);

private:
    int m_fd;
    Logger* m_logger;
    
};

template <typename Key, typename Args>
GateWay<Key, Args>::GateWay(int fd)
	: m_fd(fd), m_logger(get_logger()) { }

template <typename Key, typename Args>
int GateWay<Key, Args>::get_fd()
{
	return m_fd;
}

template <typename Key, typename Args>
GateWay<Key, Args>::~GateWay() { }

template <typename Key, typename Args>
void GateWay<Key, Args>::LOG(Logger::Severity sever, const std::string& msg, int line)
{
	m_logger->write(sever, YELLOW2 "GateWay" RESET, msg, line);
}

}//namespace ilrd

#endif //__GATEWAY_HPP__
