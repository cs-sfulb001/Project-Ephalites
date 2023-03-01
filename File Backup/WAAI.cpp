#include "WAAI.h"
#include "Choices.h"
#include "WAAIOperations.h"
#include "StringManip.h"
#include "UIHelper.h"
#include <string>
#include <vector>

void WAAI::startUp()
{
	Operations.buildTree();
	running();

}
void WAAI::running()
{
	header("Project Ephalites", 100, '_');
	cout << "To stop Project Ephalites input \"PE Stop\"" << endl;
	cout << "Each command and result will be seperated by a line" << endl;
	bool run = 1;
	while (run)
	{
		dividingLine(80, '_');
		string UserInput="";
		getline(cin, UserInput);
		if (UserInput == "PE Stop")
		{
			break;
		}
		processInput(UserInput);
	}
	shutDown();
}
void WAAI::shutDown()
{
	Operations.saveTree();
}
void WAAI::processInput(string input)
{
	std::vector<std::string>splitInput = SplitString(input);
	for (int i = 0; i < splitInput.size(); i++)
	{
		stringFormatCapitalized(splitInput[i]);
	}
	if(splitInput.size()>0)
		Operations.inputOperationDeterminor(splitInput);
}