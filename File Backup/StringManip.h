#pragma once
#include <iomanip>
#include <vector>
#include <iostream>
#include<sstream>

std::vector<std::string> SplitString(std::string);
std::string RemovePunctuation(std::string);
double StringtoDouble(std::string);
int StringtoInt(std::string);
bool hasDecimal(std::string);
void removeParethese(std::vector<std::string>&, std::vector<int>&);
void stringFormatCapitalized(std::string&);
template<class T>
std::string numberToString(T number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}