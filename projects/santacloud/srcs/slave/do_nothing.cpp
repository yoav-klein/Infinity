#include "do_nothing_slave.hpp"


namespace ilrd
{

DoNothingTask::DoNothingTask()
	: RETask(RETask::LOW) { }

std::unique_ptr<RETask> ret_do_nothing(DataArgs args, bool& should_stop)
{
	(void)args;
	(void)should_stop;
	return std::unique_ptr<RETask>(new DoNothingTask());
}


void DoNothingTask::execute()
{
	// doing nothing
}

} // ilrd


