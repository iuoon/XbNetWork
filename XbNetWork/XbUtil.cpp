#include "XbUtil.h"
#include <iostream>  
#include <sstream>

std::string xb_tostring(size_t num)
{
	std::stringstream ss;
	std::string str;
	ss << num;
	ss >> str;
	return str;
}

