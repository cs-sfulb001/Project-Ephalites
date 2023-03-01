#pragma once
#include "OperationTree.h"
#include"TreeControl.h"
#include <vector>
#include <iostream>
class WAAIOperations
{
public:
	WAAIOperations()=default;
	~WAAIOperations()=default;
	void saveTree();
	void inputOperationDeterminor(std::vector <std::string>);
	bool isOperationKeyWord(std::string&);
	void buildTree();
	void determineFunction(std::vector<std::string>, std::vector<int>);
	void determineOrder(std::vector<std::string>&, std::vector<int>&);
	void fixPriority(std::vector<std::pair<std::string, int>>, std::vector<int>&, int&);
	void fixPriority(std::vector<int>&, std::vector<std::string>);
	std::string callFuntion(std::vector<std::string>, std::vector<int>);
	std::string callFuntion(std::string, std::vector<std::string>);
	int functionToNumber(std::string);
	std::string getDefintion(std::vector<std::string>);
private:
	OperationTree FunctionTree;
	TreeControl Memory;
};

