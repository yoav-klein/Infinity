/*****************************************
	TPTask
	an interface for derived classes that will
	implement read and write operations.
	the derived muse implement a function execute()
	
	the only class that can activate execute() is 
	ThreadPool since it's a friend.
	
	
	Author: Yoav Klein
	Date: 25.11.19
	
********************************************/


#ifndef _TPTask_H_
#define _TPTask_H_

#include <memory> // std::unique_ptr

namespace ilrd
{

class TPTask
{
public:
    enum Priority
    {
        LOW,
        MEDIUM,
        HIGH,
        SUSPEND,
        STOP
    };

    TPTask(Priority p);
    virtual ~TPTask() = default;
//TODO implement operator <, how will it work with unique_ptr
//change priority queue?
	bool operator<(const TPTask& other);
   
    TPTask(const TPTask& other)             = delete;
    TPTask& operator=(const TPTask& other)  = delete;
    TPTask(TPTask&& other)                  = default;
    TPTask& operator=(TPTask&& other)       = default;
private:
    virtual void execute() = 0;
    friend class ThreadPool;
    Priority m_p;
};


}//ilrd

#endif //_TPTask_H_
