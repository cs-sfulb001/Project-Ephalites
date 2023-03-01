#include "WAAIOperations.h"
#include "StringManip.h"
#include "Functions.h"
void WAAIOperations::buildTree()
{
	FunctionTree.readFunctionFile();
	Memory.buildTrees();
}
void WAAIOperations::saveTree()
{
	Memory.SaveTrees();
}
void WAAIOperations::inputOperationDeterminor(std::vector<std::string> wordList)
{
	std::vector<int> keywordList;
	for (int i = 0; i < wordList.size(); i++)
	{
		//std::cout << i << ": " << wordList[i] <<": "<<wordList[i].size()<< std::endl;
		if (isOperationKeyWord(wordList[i]))
		{
			//std::cout << "Found keyword at: " << i << std::endl;
			keywordList.push_back(i);
		}
	}
	for (int i = 0; i < keywordList.size(); i++)
	{
		std::cout << keywordList[i] << std::endl;
	}
	determineFunction(wordList, keywordList);
}
bool WAAIOperations::isOperationKeyWord(std::string& word)
{
	BaseNode* function = FunctionTree.findFunction(word);
	if (function != nullptr)
	{
		(*function)++;
		if (!function->getTitle())
		{
			word = function->getParent()->getWord();
			function = function->getParent();
			(*function)++;
		}
		return 1;
	}
	return 0;
}
void WAAIOperations::determineFunction(std::vector<std::string> words, std::vector<int> keyWords)
{
	for (int i = 0; i < words.size(); i++)
	{
		std::cout << words[i] << std::endl;
	}
	std::string result;
	if (keyWords.size() == 1)
	{
		std::string keyword = words[keyWords[0]];
		words.erase(words.begin() + keyWords[0]);
		keyWords.clear();
		std::cout << callFuntion(keyword, words) << std::endl;
	}
	else if(keyWords.size()>1)
	{
		determineOrder(words, keyWords);
		for (int i = 0; i < keyWords.size(); i++)
		{
			std::cout << keyWords[i] << std::endl;
		}
		std::cout << "Results: "<<callFuntion(words, keyWords) << std::endl;//Order of Operations does not work at moment needs work
		keyWords.clear();
	}
	else
	{
		//Create a responce based on the input vector of strings
		std::cout << Responce(words) << std::endl;
	}
}
void WAAIOperations::determineOrder(std::vector<std::string>& words, std::vector<int>& keyWords)
{
	std::vector<std::pair<int, int> > parentheses;
	for (int i = 0; i < words.size(); i++)
	{
		if (words[i] == "(")
		{
			std::pair<int, int> temp;
			temp.first = i;
			temp.second = -1;
			parentheses.push_back(temp);
		}
		else if (parentheses.size() > 0)
		{
			if (words[i] == ")")
			{
				//std::cout << "Saving parentheses end at "<< i << std::endl;
				for (int j = (parentheses.size() - 1); j >= 0; j--)
				{
					if (parentheses[j].second == -1)
					{
						parentheses[j].second = i;
						break;
					}
				}
			}
		}
	}
	/*
	for (int i = 0; i < parentheses.size(); i++)
	{
		std::cout << parentheses[i].first << " " << parentheses[i].second << std::endl;
	}
	*/
	if (parentheses.size() == 0)
	{
		std::vector<std::string> functions;
		for (int i = 0; i < keyWords.size(); i++)
		{
			functions.push_back(words[keyWords[i]]);
		}
		int x = 0;
		fixPriority(keyWords, words);
		removeParethese(words, keyWords);
		return;
	}
	int soFar = 0;
	for (int i = parentheses.size() - 1; i >= 0; i--)
	{
		std::vector<std::pair<std::string, int>> functions;
		for (int j = 0; j < keyWords.size(); j++)
		{
			if (keyWords[j] > parentheses[i].first && keyWords[j] < parentheses[i].second)
			{
				std::pair<std::string, int> function;
				function.first = words[keyWords[j]];
				function.second = j;
				functions.push_back(function);
			}
		}
		fixPriority(functions, keyWords, soFar);
	}
	std::vector<std::pair<std::string, int>> functions;
	for (int i = soFar; i < keyWords.size(); i++)
	{
		std::pair<std::string, int> function;
		function.first = words[keyWords[i]];
		function.second = i;
		functions.push_back(function);
	}
	fixPriority(functions, keyWords, soFar);
	for (int i = 0; i < keyWords.size(); i++)
	{
		std::cout << "Key word " << i << " : " << keyWords[i] << std::endl;
	}
	removeParethese(words, keyWords);
}
void WAAIOperations::fixPriority(std::vector<std::pair<std::string, int>> functions, std::vector<int>& keywords, int& soFar)
{
	std::vector<int> priority;
	for (int i = 0; i < functions.size(); i++)
	{
		if (functions[i].first == "Multiply" || functions[i].first == "Divide" || functions[i].first == "Define")
			priority.push_back(1);
		else if (functions[i].first == "Add" || functions[i].first == "Subtract")
			priority.push_back(2);
	}
	int place = soFar;
	for (int i = 0; i < functions.size(); i++)//Moves all functions foward
	{
		std::swap(keywords[place], keywords[functions[i].second]);
	}
	for (int i = 1; i < priority.size(); i++)
	{
		bool swaped = 0;
		for (int j = 0; j < priority.size() - i; j++)
		{
			if (priority[j] < priority[j + 1])
			{
				std::swap(priority[j], priority[j + 1]);
				std::swap(functions[j], functions[j + 1]);
				std::swap(keywords[j+soFar], keywords[j + 1 + soFar]);
				swaped = 1;
			}
		}
		if (!swaped)
		{
			soFar += priority.size();
			return;
		}
	}
	soFar += priority.size();
}
void WAAIOperations::fixPriority(std::vector<int>& keywords, std::vector <std::string> words)
{
	std::vector<int> priority;
	for (int i = 0; i < keywords.size(); i++)
	{
		if (words[keywords[i]] == "Multiply" || words[keywords[i]] == "Divide" || words[keywords[i]] == "Define")
			priority.push_back(1);
		else if (words[keywords[i]] == "Add" || words[keywords[i]] == "Subtract")
			priority.push_back(2);
	}
	for (int i = 1; i < priority.size(); i++)
	{
		bool swaped = 0;
		for (int j = 0; j < priority.size() - i; j++)
		{
			if (priority[j] > priority[j + 1])
			{
				std::swap(priority[j], priority[j + 1]);
				std::swap(keywords[j], keywords[j + 1]);
				swaped = 1;
			}
		}
		if (!swaped)
		{
			return;
		}
	}
}
std::string WAAIOperations::callFuntion(std::string keyword, std::vector<std::string> words)
{
	std::cout << "Exicuting Function" << std::endl;
	std::cout << keyword<<": "<<functionToNumber(keyword) << std::endl;
	std::vector<int> intNumbers;
	std::vector<double>doubleNumbers;
	if (functionToNumber(keyword) < 5)
	{
		bool isDouble=0;
		for (int i = 0; i < words.size(); i++) 
		{
			if (hasDecimal(words[i]))
			{
				isDouble = 1;
				break;
			}
		}
		for (int i = 0; i < words.size(); i++)
		{
			if (isDouble)
			{
				doubleNumbers.push_back(StringtoDouble(words[i]));
			}
			else
			{
				intNumbers.push_back(StringtoInt(words[i]));
			}
		}
	}
	switch (functionToNumber(keyword))
	{
	default:
		break;
	case 1:
	{
		if (doubleNumbers.size() != 0)
		{
			return ToString(Add(doubleNumbers));
		}
		return ToString(Add(intNumbers));
	}
	case 2:
	{
		if (doubleNumbers.size() != 0)
		{
			return ToString(Subtract(doubleNumbers));
		}
		return ToString(Subtract(intNumbers));
	}
	case 3:
	{
		if (doubleNumbers.size() != 0)
		{
			return ToString(Multiply(doubleNumbers));
		}
		return ToString(Multiply(intNumbers));
	}
	case 4:
	{
		if (doubleNumbers.size() != 0)
		{
			return ToString(Divide(doubleNumbers));
		}
		return ToString(Divide(intNumbers));
	}
	case 5:
		return getDefintion(words);
	}
	return "";
}
std::string WAAIOperations::callFuntion(std::vector<std::string> words, std::vector<int> keywords)
{
	for (int i = 0; i < words.size(); i++)
	{
		std::cout << words[i] << std::endl;
	}
	std::string result;
	for (size_t i = 0; i < keywords.size(); i++)
	{
		
		switch (functionToNumber(words[keywords[i]]))
		{
		default:
			std::cout << "Could not find function" << std::endl;
			break;
		case 1:
		{
			//std::cout << "Does it even get here?" << std::endl;
			std::string number1 = words[keywords[i] - 1];
			std::string number2 = words[keywords[i] + 1];
			//std::cout << number1<< number2<< std::endl;
			if (hasDecimal(number1) || hasDecimal(number2))
			{
				double temp = Add(StringtoDouble(number1), StringtoDouble(number2));
				//std::cout << temp << std::endl;
				words.erase(words.begin() + keywords[i] - 1);
				words.erase(words.begin() + keywords[i] - 1);
				words[keywords[i] - 1] = ToString(temp);
				for (int j = i; j < keywords.size(); j++)
				{
					std::cout << "Nope" << std::endl;
					if (keywords[j] > keywords[i])
						keywords[j] -= 2;
				}
				break;
			}
			int temp = Add(StringtoInt(number1), StringtoInt(number2));
			//std::cout << words[keywords[i] - 1] << std::endl;
			words.erase(words.begin() + keywords[i] - 1);
			words.erase(words.begin() + keywords[i] - 1);
			words[keywords[i] - 1] = ToString(temp);
			//std::cout << words[keywords[i] - 1] << std::endl;
			for (int j = i; j < keywords.size(); j++)
			{
				if (keywords[j] > keywords[i])
					keywords[j] -= 2;
			}
			break;
		}
		case 2:
		{
			std::string number1 = words[keywords[i] - 1];
			std::string number2 = words[keywords[i] + 1];
			if (hasDecimal(number1) || hasDecimal(number2))
			{
				double temp = StringtoDouble(number1)- StringtoDouble(number2);
				words.erase(words.begin() + keywords[i] - 1);
				words.erase(words.begin() + keywords[i] - 1);
				words[keywords[i]-1] = ToString(temp);
				for (int j = i; j < keywords.size(); j++)
				{
					if (keywords[j] > keywords[i])
						keywords[j] -= 2;
				}
				break;
			}
			int temp = StringtoInt(number1)- StringtoInt(number2);
			words.erase(words.begin() + keywords[i] - 1);
			words.erase(words.begin() + keywords[i] - 1);
			words[keywords[i]-1] = ToString(temp);
			for (int j = i; j < keywords.size(); j++)
			{
				if (keywords[j] > keywords[i])
					keywords[j] -= 2;
			}
			break;
		}
		case 3:
		{
			std::string number1 = words[keywords[i] - 1];
			std::string number2 = words[keywords[i] + 1];
			if (hasDecimal(number1) || hasDecimal(number2))
			{
				double temp = StringtoDouble(number1) * StringtoDouble(number2);
				words.erase(words.begin() + keywords[i] - 1);
				words.erase(words.begin() + keywords[i] - 1);
				words[keywords[i]-1] = ToString(temp);
				for (int j = i; j < keywords.size(); j++)
				{
					if (keywords[j] > keywords[i])
						keywords[j] -= 2;
				}
				break;
			}
			int temp = StringtoInt(number1) * StringtoInt(number2);
			words.erase(words.begin() + keywords[i] - 1);
			words.erase(words.begin() + keywords[i] - 1);
			words[keywords[i]-1] = ToString(temp);
			for (int j = i; j < keywords.size(); j++)
			{
				if (keywords[j] > keywords[i])
					keywords[j] -= 2;
			}
			break;
		}
		case 4:
		{
			std::string number1 = words[keywords[i] - 1];
			std::string number2 = words[keywords[i] + 1];
			if (hasDecimal(number1) || hasDecimal(number2))
			{
				double temp = StringtoDouble(number1) / StringtoDouble(number2);
				words.erase(words.begin() + keywords[i] - 1);
				words.erase(words.begin() + keywords[i] - 1);
				words[keywords[i]-1] = ToString(temp);
				for (int j = i; j < keywords.size(); j++)
				{
					if (keywords[j] > keywords[i])
						keywords[j] -= 2;
				}
				break;
			}
			int temp = StringtoInt(number1) / StringtoInt(number2);
			words.erase(words.begin() + keywords[i] - 1);
			words.erase(words.begin() + keywords[i] - 1);
			words[keywords[i]-1] = ToString(temp);
			for (int j = i; j < keywords.size(); j++)
			{
				if (keywords[j] > keywords[i])
					keywords[j] -= 2;
			}
			break;
		}
		case 5:
		{
			 result= getDefintion(words);
		}
		}
	}
	for (int i = 0; i < words.size(); i++)
	{
		std::cout << words[i] << std::endl;
	}
	if (words.size()==1)
	{
		result = words[0];
	}
	std::cout << words[0]<<words.size() << result << std::endl;
	return result;
}
int WAAIOperations::functionToNumber(std::string function)
{

	if (function == "Add")
		return 1;
	if (function == "Subtract")
		return 2;
	if (function == "Multiply")
		return 3;
	if (function == "Divide")
		return 4;
	if (function == "Define")
		return 5;
	return -1;
}
std::string WAAIOperations::getDefintion(std::vector<std::string> words)
{
	int spot = -1;
	for (int i = 0; i < words.size(); i++)
	{
		if (words[i] == "Define")
		{
			spot = i;
		}
	}
	if (words.size() == 2)
	{
		if (spot == 1)
			return Define((Memory.GetLT()), words[0]);
		else
			return Define((Memory.GetLT()), words[1]);
	}
	else
	{
		if (words[spot + 1] == "of")
		{
			return Define((Memory.GetLT()), words[spot + 2]);
		}
		else
		{
			return Define((Memory.GetLT()), words[spot + 1]);
		}
	}
}
/*
This is gonna get real complicated. Will need to lead to learing and analysising words and returning a responce
*/
std::string WAAIOperations::Responce(std::vector<std::string> words) {
	std::vector<std::string> Unknown_words;
	for (int i = 0; i < words.size(); i++)
	{
		if (!Memory.inLTree(words[i]))
		{
			Unknown_words.push_back(words[i]);
			std::cout << words[i] << " is unknown" << std::endl;
		}
	}
	LearnWords(words, Unknown_words);
	return "I know not what you say";
}
/*
This is where all the learning of new words will take place. Will have to look at sentance structure and context clues to learn similar to how a human does
General Sentance structure:
 subject+verb+object
	Dosent help much due to the complex nature of how people speak
	subject consist of adjectives and nouns: The Fat Cat
	Can also use pronouns and adjectives: She is very fat
	Sate of being verbs: is, was, has
	Need to do a deep dive on anlysising english before I can program something to anlysis english
*/
void WAAIOperations::LearnWords(std::vector<std::string> words, std::vector<std::string> unknownWords) {
	
}