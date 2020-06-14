
#include "tptask.hpp" // TPTask

namespace ilrd
{

class CatTask : public TPTask
{
public:
	CatTask(std::string name, int legs);
	
	void execute() override;

private:
	std::string m_name;
	int m_legs;
};

} // ilrd
