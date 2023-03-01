#pragma once
#include "TreeControl.h"
#include "WAAIOperations.h"

using namespace std;
class WAAI
{
public:
	void startUp();
	void running();
	void shutDown();
	void processInput(string);
private:
	WAAIOperations Operations;

};

