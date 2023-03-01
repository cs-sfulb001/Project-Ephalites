#include "Options.h"
#include"StringManip.h"
#include "Choices.h"
#include "UIHelper.h"
#include <fstream>
#include<string>
#include<vector>
Options::Options() :Options(0, 2){}
Options::Options(int min, double max)
{
	_minTimesUsed = min;
	_MaxRAM = max;
}
void Options::minTimesUsed(int min)
{
	_minTimesUsed = min;
}
int Options::minTimesUsed()
{
	return _minTimesUsed;
}
void Options::MaxRAM(double max)
{
	_MaxRAM = max;
}
double Options::MaxRAM()
{
	return _MaxRAM;
}
void Options::printOptions()
{
	bool run = 1;
	while (run)
	{
		header("Options", 80, '_');
		std::cout << "The minimum times used to be stored in ram is " << _minTimesUsed << std::endl;
		std::cout << "The maxium RAM that can be used is " << _MaxRAM << "GB" << std::endl;
		std::cout << "Would you like to: " << std::endl;
		switch (choiceList({ "Change minium times used", "Change the maxium RAM", "Exit" }))
		{
		default:
			std::cout << "That is not an option" << std::endl;
			break;
		case 0:
		{
			std::cout << "How many times will be the new minimum" << std::endl;
			int input = 0;
			std::cin >> input;
			std::cin.clear();
			minTimesUsed(input);
			break;
		}
		case 1:
		{
			std::cout << "How many GBs will be the new maxium" << std::endl;
			double input = 0;
			std::cin >> input;
			std::cin.clear();
			MaxRAM(input);
			break;
		}
		case 2:
			run = 0;
			break;
		}
	}
}
void Options::LoadFile()
{
	std::ifstream OptionFile;
	OptionFile.open("OptionFile.txt");
	std::string input;
	std::getline(OptionFile, input);
	std::vector<std::string> line;
	line = SplitString(input);
	if(line.size()>=1)
		minTimesUsed(StringtoInt(line[line.size() - 1]));
	line.clear();
	std::getline(OptionFile, input);
	line = SplitString(input);
	if(line.size()>=2)
		MaxRAM(StringtoDouble(line[line.size() - 2]));
	OptionFile.close();
}
void Options::SaveFile()
{
	std::ofstream OptionFile;
	OptionFile.open("OptionFile.txt");
	OptionFile << "The minimum times used to be stored in ram is " << minTimesUsed() << std::endl;
	OptionFile << "The maxium RAM that can be used is " << MaxRAM() << " GB" << std::endl;
}