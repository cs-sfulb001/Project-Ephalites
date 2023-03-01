#include "OperationTree.h"
#include <fstream>
void OperationTree::readFunctionFile()
{
	std::ifstream FunctionIn;
	FunctionIn.open("FunctionTree.txt");
    std::string parent = "";
    std::string word = "";
    bool title = 0;
    int times = 0;
    std::string Misc = "";
    while (true)
    {
        if (word == "")
        {
            FunctionIn >> word >> title >> times >> Misc;
        }
        else
        {
            FunctionIn >> title >> times >> Misc;
        }
        if (word == "")
        {
            break;
        }
        BaseNode* WordAdress = findFunction(word);
        if (title && WordAdress == nullptr)
        {
            newFunctionNode(word, times);
        }
        else if (WordAdress == nullptr)
        {
            newSynonym(word, parent, times);
        }
        if (Misc != "->")
        {
            parent = "";
            word = Misc;
            Misc = "";
        }
        else
        {
            parent = word;
            word = "";
            Misc = "";
        }
    }
}
BaseNode* OperationTree::findFunction(std::string targetWord)
{
    return findFunction(targetWord, &functions);
}
BaseNode* OperationTree::findFunction(std::string targetWord, BaseNode* currentNode)
{
    if (currentNode->getWord() == targetWord)
        return currentNode;
    for (int i = 0; i < currentNode->getNumChildren(); i++)
    {
        BaseNode* NodeReturn = nullptr;
        NodeReturn = findFunction(targetWord, currentNode->getChild(i));
        if (NodeReturn != nullptr)
            return NodeReturn;
    }
    return nullptr;
}
void OperationTree::newFunctionNode(std::string function, int timesUsed)
{
    functions.addChild(function, 1, timesUsed);
}
void OperationTree::newSynonym(std::string functionSynonym, std::string function, int timesUsed)
{
    BaseNode* parent = findFunction(function);
    if (parent == nullptr)
    {

        newFunctionNode(function, 0);
        parent = findFunction(function); 
        
    }
    parent->addChild(functionSynonym, 0, timesUsed);
    
}