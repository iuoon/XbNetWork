#pragma once
#include <string>  

std::string base64_encode(char const*, int len);
std::string base64_decode(std::string const& s);