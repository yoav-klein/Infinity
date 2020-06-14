/***************************************************
	Interface for Monitor classes
	
	contains an enum of event type. to the moment it's three:
	read, write, and hangup. 
	
	class Event is what's returned from operator[] so the user
	can retrieve the fd that signals and the type of event.


***************************************************************/


#ifndef __MONITOR_HPP__
#define __MONITOR_HPP__

#include <iostream>

namespace ilrd
{

class Event;

class Monitor
{
public:
	enum event
	{
		READ,
		WRITE,
		HUP  // hangup
	};
	
	Monitor() = default;
	virtual ~Monitor() = 0;
	
	virtual void add(int fd, event event) = 0;
	virtual void remove(int fd) = 0;
	virtual int wait() = 0;
	virtual Event operator[](std::size_t index) = 0;
	
	Monitor(const Monitor& other) = delete;
	Monitor& operator=(const Monitor& other) = delete;
	Monitor(Monitor&& other) = delete;
	Monitor& operator=(Monitor&& other) = delete;
};


class Event
{
public:
	Event(int fd, Monitor::event event);
	int get_fd();
	Monitor::event get_event();
	
private:
	int m_fd;
	Monitor::event m_event;
};


} // namespace ilrd

#endif // __MONITOR_HPP__
