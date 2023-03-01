#pragma once
#include <iostream>
#include <vector>
#include "BaseNode.h"
class OperationTree
{
public:
	void readFunctionFile();
	BaseNode* findFunction(std::string);
	BaseNode* findFunction(std::string, BaseNode*);
	void newFunctionNode(std::string, int);
	void newSynonym(std::string, std::string, int);
private:
	BaseNode functions;
};

