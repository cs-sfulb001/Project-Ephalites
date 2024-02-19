#include "BaseNode.h"
BaseNode::BaseNode():BaseNode("-Blank-", 0){}
BaseNode::BaseNode(std::string word, bool title):BaseNode(word, title, 0){}
BaseNode::BaseNode(std::string word, bool title,int times)
{
	Word = word;
	Title = title;
	Parent = nullptr;
	timesUsed = times;
}
BaseNode::BaseNode(std::string word, bool title, BaseNode& parentNode):BaseNode(word,title, parentNode, 0){}
BaseNode::BaseNode(std::string word, bool title, BaseNode& parentNode, int times)
{
	Word = word;
	Title = title;
	Parent = &parentNode;
	timesUsed = times;

}
BaseNode::~BaseNode()
{
	if (Children.size() > 0&&Parent!=nullptr)
	{
		for (int i = 0; i < Children.size(); i++)
		{
			Children[i]->setParent(*Parent);
			Parent->addChild(*Children[i]);
		}
	}
	else if (Parent == nullptr&&Children.size()>0)
	{
		for (int i = 0; i < Children.size(); i++)
		{
			delete Children[0];
		}
	}
	Children.clear();
	if(Parent!=nullptr)
		Parent->removeChild(Word);
	Parent = nullptr;
}
//gets
std::string const BaseNode::getWord()
{
	return Word;
}
bool const BaseNode::getTitle()
{
	return Title;
}
BaseNode* const BaseNode::getParent()
{
	return *&Parent;
}
BaseNode* const BaseNode::getChild(std::string word)
{
	return *&Children[std::hash<std::string>()(word)];
}
std::unordered_map<int, BaseNode*> const BaseNode::getChildren()
{
	return Children;
}
int const BaseNode::getNumChildren()
{
	return Children.size();
}

CatNode* const BaseNode::getCTEqiv()
{
	return nullptr;
}

int const BaseNode::getTimesUsed()
{
	return timesUsed;
}
std::string const BaseNode::getDef()
{
	return Def;
}
std::string& BaseNode::getWordRef()
{
	return Word;
}
int& BaseNode::getTimesUsedRef()
{
	return timesUsed;
}
//Sets
void BaseNode::setWord(std::string newWord)
{
	Word = newWord;
}
void BaseNode::setTitle(bool newTitle)
{
	Title = newTitle;
}
void BaseNode::setParent(BaseNode& ParentNode)
{
	Parent = &ParentNode;
}
void BaseNode::setDef(std::string newDef)
{
	Def = newDef;
}
/*
For adding children under the current node.
*/
void BaseNode::addChild(std::string childWord, bool childTitle, int times)
{
	BaseNode* newChild = new BaseNode(childWord, childTitle, *this, times);
	int hashvalue = std::hash<BaseNode>()(*newChild);
	Children.insert(std::make_pair(hashvalue, newChild));
}
void BaseNode::addChild(BaseNode& newChild)
{
	int hashvalue = std::hash<BaseNode>()(newChild);
	Children.insert(std::make_pair(hashvalue, &newChild));
}
void BaseNode::setTimesUsed(int times)
{
	timesUsed = times;
}
//Remove
void BaseNode::removeChild(std::string target)
{
	int targetHash = std::hash<std::string>()(target);
	Children.erase(targetHash);
}
void BaseNode::removeParent()
{
	Parent = nullptr;
}
std::unordered_map<int, BaseNode*>::iterator BaseNode::getChildernBegin() {
	return Children.begin();
}
std::unordered_map<int, BaseNode*>::iterator BaseNode::getChildernEnd() {
	return Children.end();
}
//Data Manip
//Depercated due to switch to unordered map from vector
/*
void BaseNode::sortChildren()
{
	for (int i = 0; i < Children.size(); i++)
	{
		bool moved = 0;
		for (int j = 0; j < Children.size()-i; j++)
		{
			if (Children[i] < Children[j])
			{
				BaseNode* temp = Children[i];
				Children[i] = Children[j];
				Children[j] = temp;
				moved = 1;
			}
		}
		if (!moved)
			return;
	}
}*/
//Operator overload

int BaseNode::operator++(int)
{
	if (timesUsed < 2147483647) 
	{
		int temp = timesUsed++;
		//this->Parent->sortChildren();
		return temp;
	}
	return timesUsed;
}