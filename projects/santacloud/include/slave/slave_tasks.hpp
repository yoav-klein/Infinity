
#ifndef __SLAVE_TASKS_HPP__
#define __SLAVE_TASKS_HPP__

#include "storage.hpp" // Storage
#include "driver_proxy.hpp" // ProxyDriver
#include "retask.hpp" // RETask
#include "data_args.hpp" // DataArgs

namespace ilrd
{


template <typename T>
std::unique_ptr<RETask> slave_create(DataArgs args, bool& should_stop)
{
	return std::unique_ptr<RETask>(new T(std::move(args), should_stop));
}

// for StdinGate
std::unique_ptr<RETask> stop_running(DataArgs args, bool& should_stop);


class SlaveTask : public RETask
{
public:
	SlaveTask(DataArgs args, bool& should_stop);
	
	virtual void execute() = 0;

protected:
	ProxyDriver* m_driver;
	Storage* m_storage;
	std::unique_ptr<DriverData> m_data;
	bool& m_should_stop;
};

class ReadTask : public SlaveTask
{
public:
	ReadTask(DataArgs args, bool& should_stop);
	
	void execute() override;
};

class WriteTask : public SlaveTask
{
public:
	WriteTask(DataArgs args, bool& should_stop);
	
	void execute() override;

};





}


#endif // __SLAVE_TASKS_HPP__
