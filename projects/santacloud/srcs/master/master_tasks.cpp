#include "do_nothing_master.hpp" // DoNothingTask
#include "master_tasks.hpp"

namespace ilrd
{

std::unique_ptr<RETask> call_disconnect(MasterArgs args, bool& should_stop)
{
	(void)should_stop;
	args.m_driver->disconnect();
	
	return std::unique_ptr<RETask>(new DoNothingTask());
}

MasterTask::MasterTask(MasterArgs args, bool& should_stop)
	:  RETask(RETask::HIGH), m_driver(args.m_driver), m_kamikaze(args.m_kamikaze), 
	   m_data(args.m_data.release()), m_should_stop(should_stop)
{
}

SendRequestTask::SendRequestTask(MasterArgs args, bool& should_stop)
	: MasterTask(std::move(args), should_stop)
{ }

SendReplyTask::SendReplyTask(MasterArgs args, bool& should_stop)
	: MasterTask(std::move(args), should_stop)
{ }

DiscTask::DiscTask(MasterArgs args, bool& should_stop)
	: MasterTask(std::move(args), should_stop)
{ }

OtherTask::OtherTask(MasterArgs args, bool& should_stop)
	: MasterTask(std::move(args), should_stop)
{ }

void SendRequestTask::execute()
{
	m_kamikaze->send_request(std::move(m_data));
}

void SendReplyTask::execute()
{
	m_driver->send_reply(std::move(m_data));
}
	
void DiscTask::execute()
{
	m_should_stop = true;
	m_driver->send_reply(std::move(m_data));
}
	
void OtherTask::execute()
{
	m_driver->send_reply(std::move(m_data));
}



} // ilrd
