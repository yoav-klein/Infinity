
#include "retask.hpp"

namespace ilrd
{

RETask::RETask(Priority p)
	: TPTask(static_cast<TPTask::Priority>(p)) { }
	
} // ilrd
