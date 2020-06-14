#include "handleton.hpp"
#include "logger.hpp"
#include "factory.hpp"
#include "retask.hpp"
#include "data_args.hpp"
#include "config_data.hpp"

namespace ilrd
{

InitHandleton(Logger)

using TasksFactory = Factory<RETask, int, std::shared_ptr<DataArgs>, bool&>;

InitHandleton(TasksFactory)
InitHandleton(ConfigDataSlave)
InitHandleton(ConfigDataMaster)

Logger* get_logger()
{
	return Handleton<Logger>::get_instance();
}



} // namespace ilrd
