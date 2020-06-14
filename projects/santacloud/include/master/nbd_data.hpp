/*****************************************************
	NBDData
	
	this class is derived from DriverData,
	and adds to it two fields: 
	-	char m_handle[8]
	-	int status;
	
	it's for to carry data of NBD requests and replies.

	Author: Yoav Klein
	Date: 8.12.19

*******************************************************/

#ifndef __NBD_DATA_HPP__
#define __NBD_DATA_HPP__

#include "driver_data.hpp"

namespace ilrd
{

class NBDData : public DriverData
{
public:
	NBDData(action_type type, char* handle, size_t offset, unsigned int length,  
			int req_id = 0);
	
	char* get_handle();

private:
	static const int HANDLE_SIZE = 8;
	
	char m_handle[HANDLE_SIZE];


};


} // ilrd



#endif // #define __NBD_DATA_HPP__
