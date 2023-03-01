#include "Functions.h"

std::string Define(LanguageTree* trees, std::string word)
{
	BaseNode* wordPointer = trees->findWord(word);
	if (wordPointer == nullptr)
	{
		std::cout << "Not in tree" << std::endl;
	}
	if (wordPointer != nullptr)
		return wordPointer->getWord() + " - " + wordPointer->getDef();
	return (char)34 + word +(char)34+ " Not Found";
}