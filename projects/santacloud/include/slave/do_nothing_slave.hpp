
#ifndef __DO_NOTHING_HPP__
#define __DO_NOTHING_HPP__

#include "retask.hpp" // RETask
#include "data_args.hpp" // DataArgs

namespace ilrd
{

std::unique_ptr<RETask> ret_do_nothing(DataArgs args, bool& should_stop);

class DoNothingTask : public RETask
{
public:
	DoNothingTask();
	
	void execute();

};





} // ilrd


#endif // #ifndef __DO_NOTHING_HPP__
