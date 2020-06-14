
#include "tptask.hpp" // TPTask

namespace ilrd
{

class DogTask : public TPTask
{
public:
	DogTask(std::string name, int legs);
	
	void execute() override;

private:
	std::string m_name;
	int m_legs;
};

} // ilrd
