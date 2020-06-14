#ifndef __RETASK_H__
#define __RETASK_H__

#include "tptask.hpp"

/** Inheriting class needs to provide an execute function:
 * void execute();
 * This function will be used to execute the task.
 *
 * and a static create function:
 * static std::unique_ptr<RETask> create(Args...);
 * This function returns a unique_ptr to the new task; 
 */

namespace ilrd
{

class RETask : public TPTask
{
public:
    enum Priority
    {
        LOW = TPTask::Priority::LOW,
        MEDIUM = TPTask::Priority::MEDIUM,
        HIGH = TPTask::Priority::HIGH
    };

    RETask(Priority p);
    virtual ~RETask() = default;

    //RETask::Priority get_priority() const;

    RETask(const RETask& other)             = delete;
    RETask& operator=(const RETask& other)  = delete;
    RETask(RETask&& other)                  = default;
    RETask& operator=(RETask&& other)       = default;

private:
   
    virtual void execute() = 0;
};

}//namespace ilrd

#endif //__RETASK_H__
