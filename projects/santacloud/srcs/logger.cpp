#include <fstream> // ofstream
#include <iomanip>  // stew
#include <sstream> // ostringstream
#include <ctime> // time
#include <thread> // this_thread

#include "logger.hpp"

namespace ilrd
{

Logger::Logger(Severity initial_severity, std::ostream& initial_stream)
					: m_severity(initial_severity)
{
	static std::ofstream dev_null("/dev/null");
	m_streams.push_back(&dev_null);
	m_streams.push_back(&initial_stream);
}

void Logger::write(Severity msg_severity, 
			   const std::string& module,
			   const std::string& msg,
			   int line)
{
	std::ostringstream string;
	
	#ifndef RPI // puttime doesn't work with rpi cross-compiler for some reason
	std::time_t result = std::time(nullptr);
	// format the string
	string << std::put_time(std::localtime(&result), "%H:%M:%S") << ": ";
	#endif
	
	string << "[" << std::setw(3) << std::left << line << "]" << " ";
	string << std::setw(13) << std::left << module << ": ";
	string << std::setw(25) << std::left << msg << " "; 
	string << "thread: " << std::this_thread::get_id();
	
	int should_write = msg_severity >= m_severity;
	
	// only if should_write evaluates to 1, the string will be written 
	// to the elements in the vector, otherwise, it'll be written to the
	// 0th element, which is dev/null.
	// TODO: not sure that this is better then if
	
	std::lock_guard<std::mutex> lock(m_mutex);
	for(std::size_t i = 1; i < m_streams.size(); ++i)
	{
		*m_streams[should_write * i] << string.str() << std::endl;
	}
}

void Logger::set_output_severity(Severity output_severity)
{
	m_severity = output_severity;
}

void Logger::add_output(std::ostream& output)
{
	m_streams.push_back(&output);
}



} // ilrd
