#include <iostream>
#include <iomanip>
#include <string>
#include "UIHelper.h"
void dividingLine(int length, char symbol)
{
	dividingLine(length, symbol, std::cout);
}
void dividingLine(int length, char symbol, std::ostream& out)
{
	out << dividingLineReturn(length, symbol);
}
std::string dividingLineReturn(int length, char symbol)
{
	std::string out = "";
	for (int i = 0; i < length; i++)
	{
		out += symbol;
	}
	out += "\n";
	return out;
}
void header(std::string heading, int length, char symbol)
{
	system("cls");
	dividingLine(length, symbol);
	std::cout << std::setw((length+heading.size()) / 2) << heading << std::endl;
	dividingLine(length, symbol);
}
void waitTillEnter()
{
	std::cout << "Press Enter to Continue" << std::endl;
	std::string junk;
	std::getline(std::cin, junk);
	std::cin.clear();
}