#pragma once
#include <vector>
#include <iostream>
#include "LanguageTree.h"
#include <iostream>
template<class T>
T Add(T number1, T number2)
{
	return number1 + number2;
}
template<class T>
T Add(std::vector<T> numbers)
{
	int numbersSize = numbers.size();
	for (int i = 1; i < numbersSize; i++)
	{
		numbers[0] = numbers[0] + numbers[1];
		numbers.erase(numbers.begin()+1);
	}
	return numbers[0];
}
template<class T>
T Multiply(std::vector<T> numbers)
{
	int numbersSize = numbers.size();
	for (int i = 1; i < numbersSize; i++)
	{
		numbers[0] = numbers[0] * numbers[1];
		numbers.erase(numbers.begin() + 1);
	}
	return numbers[0];
}
template<class T>
T Divide(T number1, T number2)
{
	return number1 / number2;
}
template<class T>
T Divide(std::vector<T> numbers)
{
	int numbersSize = numbers.size();
	for (int i = 1; i < numbersSize; i++)
	{
		numbers[0] = numbers[0] / numbers[1];
		numbers.erase(numbers.begin() + 1);
	}
	return numbers[0];
}
template<class T>
T Subtract(std::vector<T> numbers)
{
	int numbersSize = numbers.size();
	for (int i = 1; i < numbersSize; i++)
	{
		numbers[0] = numbers[0] - numbers[1];
		numbers.erase(numbers.begin() + 1);
	}
	return numbers[0];
}
std::string Define(LanguageTree* trees, std::string word);