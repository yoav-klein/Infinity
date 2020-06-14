#include "do_nothing_slave.hpp" // DoNothingTask
#include "slave_tasks.hpp"

namespace ilrd
{

std::unique_ptr<RETask> stop_running(DataArgs args, bool& should_stop)
{
	should_stop = true;
	(void)args;
	
	return std::unique_ptr<RETask>(new DoNothingTask());
}

SlaveTask::SlaveTask(DataArgs args, bool& should_stop)
	: RETask(RETask::HIGH), m_driver(args.m_driver), m_storage(args.m_storage), 
	  m_data(args.m_data.release()), m_should_stop(should_stop)
{
}

ReadTask::ReadTask(DataArgs args, bool& should_stop)
	: SlaveTask(std::move(args), should_stop)
{
}

WriteTask::WriteTask(DataArgs args, bool& should_stop)
	: SlaveTask(std::move(args), should_stop)
{
}

void ReadTask::execute()
{
	std::unique_ptr<DriverData> reply = m_storage->read(std::move(m_data));
	m_driver->send_reply(std::move(reply));
}

void WriteTask::execute()
{
	std::unique_ptr<DriverData> reply = m_storage->write(std::move(m_data));
	m_driver->send_reply(std::move(reply));
}

} // ilrd
