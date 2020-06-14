
#include "monitor.hpp"

namespace ilrd
{

Event::Event(int fd, Monitor::event event)
	: m_fd(fd), m_event(event) { }

int Event::get_fd()
{
	return m_fd;
}

Monitor::event Event::get_event()
{
	return m_event;
}

Monitor::~Monitor() { }


} // namespace ilrd
