/*****************************************
	Kamikaze
	
	This class is responsible for the communication
	with the slaves in the master module.

*****************************************/

#ifndef __KAMIKAZE_HPP__
#define __KAMIKAZE_HPP__

#include <memory> // std::unique_ptr
#include <unordered_map> // std::unordered_map
#include <thread> // std::thread

#include "logger.hpp" // Logger
#include "driver_data.hpp" // DriverData
#include "file_raii.hpp" // FileDescriptor
#include "config_data.hpp" // ConfigDataMaster
#include "waitable_pqueue.hpp" // WaitablePQueue

namespace ilrd
{

struct LiveRequest
{
	LiveRequest() = default;
	LiveRequest(std::shared_ptr<DriverData> data, bool is_backup);
	~LiveRequest() = default;
	
	LiveRequest(const LiveRequest& other) = default;
	LiveRequest& operator=(const LiveRequest& other) = default;
	
	std::shared_ptr<DriverData> m_data;
	bool m_is_backup;
};

struct RequestTime
{
	RequestTime() = default;
	RequestTime(int req_id, 
		std::chrono::time_point<std::chrono::high_resolution_clock> time_stamp);
	~RequestTime() = default;
	
	RequestTime(const RequestTime& other) = default;
	RequestTime& operator=(const RequestTime& other) = default;
	
	int m_req_id;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_time_stamp;
};

class Kamikaze
{
public:
	Kamikaze();
	~Kamikaze();

	void send_request(std::unique_ptr<DriverData> data);
	std::unique_ptr<DriverData> receive_reply(int fd);

	int get_socket_fd(int index);
	
private:
	static const std::size_t MAX_HEADER_SIZE = 40;
	
	void connect_to_slaves();
	void pong_slave(int socket);
	void send_request_imp(std::shared_ptr<DriverData> data, bool is_backup);
	void LOG(Logger::Severity sever, const std::string& msg, int line);
	void tracker();
	
	ConfigDataMaster* m_conf_data;
	const std::size_t MAX_MSG_SIZE;
	std::vector<int> m_sockets;
	std::vector<std::mutex> m_mutexes;
	
	std::mutex m_map_mutex;
	std::unordered_map<int, LiveRequest> m_live_reqs;
	std::thread m_track_thread;
	WaitablePQueue<RequestTime> m_time_queue;
	bool m_should_run;
	
	Logger* m_logger;
	
};


} // ilrd


#endif // __KAMIKAZE_HPP__
