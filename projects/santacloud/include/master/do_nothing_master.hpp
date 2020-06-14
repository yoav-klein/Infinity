
#ifndef __DO_NOTHING_HPP__
#define __DO_NOTHING_HPP__

#include "retask.hpp" // RETask
#include "master_args.hpp" // MasterArgs

namespace ilrd
{

std::unique_ptr<RETask> ret_do_nothing(MasterArgs args, bool& should_stop);

class DoNothingTask : public RETask
{
public:
	DoNothingTask();
	
	void execute();

};





} // ilrd


#endif // #ifndef __DO_NOTHING_HPP__
