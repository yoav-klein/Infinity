
/*****************************************************8
	MasterTasks
	
	the new master tasks, inheriting from RETask to 
	conform the Request Engine.
	
	ctor of MasterTask receive MasterArgs as argument, which
	is a struct with ProxyDriver*, Storage*, and unique_ptr<DriverData>.
	it initializes the members from this struct.
	
	This version is working with NBDData instead of DriverData
	
	24.12.19: this version works with MasterArgs instead of MasterArgs,
	since we now want to work with Kamikze which is not a derived of
	Storage.
	
	Author: Yoav Klein
	Date: 8.12.19

**********************************************************/

#ifndef __MASTER_TASKS_HPP__
#define __MASTER_TASKS_HPP__


#include <memory> // std::unique_ptr

#include <iostream>
#include "driver_proxy.hpp" // ProxyDriver
#include "driver_data.hpp" // DriverData 
#include "retask.hpp" // RETask
#include "kamikaze.hpp" // Kamikaze
#include "master_args.hpp" // MasterArgs


namespace ilrd
{


template <typename T>
std::unique_ptr<RETask> master_create(MasterArgs args, bool& should_stop)
{
	return std::unique_ptr<RETask>(new T(std::move(args), should_stop));
}

std::unique_ptr<RETask> call_disconnect(MasterArgs args, bool& should_stop);

class MasterTask : public RETask
{
public:
	MasterTask(MasterArgs args, bool& should_stop);
	
	virtual void execute() = 0;
	
protected:
	ProxyDriver* m_driver;
	Kamikaze* m_kamikaze;
	std::unique_ptr<DriverData> m_data;
	bool& m_should_stop;
};

class SendRequestTask : public MasterTask
{
public:
	SendRequestTask(MasterArgs args, bool& should_stop);
	
	void execute() override;
};

class SendReplyTask : public MasterTask
{
public:
	SendReplyTask(MasterArgs args, bool& should_stop);
	
	void execute() override;
};

class DiscTask : public MasterTask
{
public:
	DiscTask(MasterArgs args, bool& should_stop);
	
	void execute() override;
};

class OtherTask : public MasterTask
{
public:
	OtherTask(MasterArgs args, bool& should_stop);
	
	void execute() override;
};

} // ilrd

#endif  //__MASTER_TASKS_HPP__

