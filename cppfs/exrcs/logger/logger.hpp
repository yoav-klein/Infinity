#include <iostream> // std::cerr
#include <mutex> // std::mutex
#include <vector> // std::vector

#include "handleton.hpp" // Handleton<Logger>

#ifndef __LOGGER_H__
#define __LOGGER_H__

/*

* an improved version of the Logger.

* the Logger writes messages composed of: time, line number, module,
* message, and thread id.

* the Logger object holds a vector of std::ostream*, and the user 
* can add ostream* objects that the log writes will be written to.

* the write function is thread safe, using the m_mutex member of type
* std::mutex

*/

namespace ilrd
{

class Logger
{
public:
	enum Severity { DEBUG, INFO, WARNING, ERROR, NUM_SEVERITIES };
	
	
	void write(Severity msg_severity, 
			   const std::string& module,
			   const std::string& msg,
			   int line);
	
	void set_output_severity(Severity output_severity);
	void add_output(std::ostream& output);

private:
	explicit Logger(Severity initial_severity = WARNING, 
					std::ostream& initial_stream = std::cerr);
	friend class Handleton<Logger>;
	Severity m_severity;
	std::vector<std::ostream*> m_streams;
	std::mutex m_mutex;
};


} //  ilrd
#endif //__LOGGER_H__
