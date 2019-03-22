#include "StringFunctions.h"

#include <sstream>

StringFunctions::StringFunctions()
{
  //ctor
}

StringFunctions::~StringFunctions()
{
  //dtor
}

std::vector<std::string> StringFunctions::explode(const std::string& s, const char& c) {
	std::string buff{""};
	std::vector<std::string> v;

	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);

	return v;
}


// Convert int to string
std::string StringFunctions::IntToString(int value){
  std::stringstream ss;
  ss << value;
  return ss.str();
}

//Convert double to string
std::string StringFunctions::DoubleToString(double value){
  std::stringstream ss;
  ss << value;
  return ss.str();
}

//Convert bool to string
std::string StringFunctions::BoolToString(bool value){
  std::stringstream ss;
  ss << value;
  return ss.str();
}
