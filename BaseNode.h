#ifndef BASENODE_H
#define BASENODE_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include "CatNode.h"

/*
Basic Node type for trees that will be used for
Language tree and more.
*/
class BaseNode
{
public:
	//Constructors

	/*
	Default constructor
	*/
	BaseNode();
	/*
	Copy constructor
	*/
	//BaseNode(const BaseNode&);

	/*
	Constructor that takes in the word the node will store and
	the Title status.
	*/
	BaseNode(std::string, bool);
	BaseNode(std::string, bool, int);
	/*
	Constructor that takes in the word the node will store,
	the title status and the parent node.
	*/
	BaseNode(std::string, bool, BaseNode&);
	BaseNode(std::string, bool, BaseNode&, int);
	virtual ~BaseNode();
	//Gets

	std::string const getWord();
	bool const getTitle();
	BaseNode* const getParent();
	BaseNode* const getChild(std::string);
	std::unordered_map<int, BaseNode*> const getChildren();
	int const getNumChildren();
	virtual CatNode* const getCTEqiv();
	int const getTimesUsed();
	std::string const getDef();
	std::string& getWordRef();
	int& getTimesUsedRef();
	std::unordered_map<int, BaseNode*>::iterator getChildernBegin();
	std::unordered_map<int, BaseNode*>::iterator getChildernEnd();
	//Sets
	void setParent(BaseNode&);
	void setWord(std::string);
	void setDef(std::string);
	void setTitle(bool);
	void addChild(std::string, bool, int);
	void addChild(BaseNode&);
	void setTimesUsed(int);
	//Remove
	/*
	Removes input word from child vector if it is in it.
	Does not delete the word thus can lead to memory leaks.
	*/
	void removeChild(std::string);
	/*
	Sets parent to null pointer
	*/
	void removeParent();
	//Data Manip
	//Depercated due to switch to unordered map from vector
	//void sortChildren();
	//Opereator Overloads
	friend std::ostream& operator<<(std::ostream& out, const BaseNode& target)
	{
		out << target.Word << " " << target.Title <<" "<<target.timesUsed;
		return out;
	}
	int operator++(int);
	std::string Word;
private:

	bool Title;
	BaseNode* Parent;
	std::unordered_map<int, BaseNode*> Children;
	int timesUsed;
	std::string Def;
};

template<>
struct std::hash<BaseNode>
{
	std::size_t operator()(const BaseNode& node) const noexcept
	{
		return std::hash<std::string>{}(node.Word); // or use boost::hash_combine
	}
};
#endif