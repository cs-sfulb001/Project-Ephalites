#include "CatNode.h"
CatNode::CatNode()
{
	word = new std::string("Something went wrong");
    title = 0;
	isCollision = 0;
    TimesUsed = nullptr;
}

CatNode::CatNode(std::string word, int title, int times)
{
    this->word = new std::string(word);
    this->title = title;
    isCollision = 0;
    this->TimesUsed = new int(times);
}
CatNode::CatNode(std::string& word, int& times)
{
    this->word = &word;
    this->title = 0;
    isCollision = 0;
    this->TimesUsed = &times;
}
/*CatNode::~CatNode()
{

    if (Children.size() != 0)
    {
        for (int i = 0; i < Children.size(); i++)
        {
            if (Children[i]->getNumParents() > 1)
            {
                Children[i]->removeParent(*word);
            }
        }
        Children.clear();
    }
    if (Parents.size() != 0)
    {
        for (int i = 0; i < Parents.size(); i++)
        {
            Parents[i]->removeChild(*word);
        }
    }
    Parents.clear();
    word = nullptr;
    TimesUsed = nullptr;
}*/
//Sets
void CatNode::setCollisionStatus(bool newStatus)
{
    isCollision = newStatus;
}
//Gets
std::string const CatNode::getWord()
{
	return *word;
}
int const CatNode::getTimesUsed()
{
    return *TimesUsed;
}
/*
int const CatNode::getNumChildren()
{
	return Children.size();
}
int const CatNode::getNumParents()
{
	return Parents.size();
}
*/
CatNode* const CatNode::getChild(std::string word)
{
	return Children[std::hash<std::string>{}(word)];
}
CatNode* const CatNode::getParent(std::string word)
{
	return Parents[std::hash<std::string>{}(word)];
}
bool const CatNode::getIsCollision()
{
	return isCollision;
}
bool const CatNode::isParent(std::string target)
{
    return Parents[std::hash<std::string>{}(target)] != nullptr;
}

bool const CatNode::isParent(CatNode& target)
{
    return Parents[std::hash<CatNode>{}(target)]== &target;
}
bool const CatNode::getAnyCollisionParents()
{
    if (CollisionParent.size() != 0)
        return true;
    return false;
}
std::vector<std::string> CatNode::getCollisionList(int slot)
{
    return CollisionParent[slot];
}
bool const CatNode::getAnyParents()
{
    return !(Parents.empty());
}
//Adds
void CatNode::AddCollisionParent(std::vector <std::string> CCP)
{
    if (CCP.size() == 0)
    {
        return;
    }
    int slot = -1;
    for (int i = 0; i < CollisionParent.size(); i++)
    {
        if (CollisionParent[i][0] == CCP[0])
        {
            for (int j = 0; j < CollisionParent[i].size()&&j<CCP.size(); j++)
            {
                if (CollisionParent[i][j] != CCP[j])
                {
                    break;
                }
                if (j == CollisionParent[i].size() - 1)
                    slot = i;
            }
            if (slot != -1)
                break;
        }
    }
    if (slot != -1)
    {
        int i = CCP.size() - 1;
        CollisionParent[slot].push_back(CCP[i]);
    }
    else
    {
        CollisionParent.push_back(CCP);
    }
}
void CatNode::AddCollisionParent(std::vector<std::string> CCP, int newCollisionPoint)
{
    int slot = -1;
    for (int i = 0; i < CollisionParent.size(); i++)
    {
        if (CollisionParent[i][0] == CCP[0])
        {
            int augment = 0;
            for (int j = 0; j < CCP.size(); j++)
            {
                if (j == newCollisionPoint)
                    augment++;
                if (augment+j>=CCP.size()||CollisionParent[i][j] != CCP[j + augment])
                {
                    break;
                }
                if (j == CCP.size() - 1)
                {
                    slot = i;
                }
            }
            if (slot != -1)
                break;
        }
    }
    if (slot == -1)
    {
        CollisionParent.push_back(CCP);
    }
    else
    {
        CollisionParent.erase(CollisionParent.begin());
        CollisionParent.push_back(CCP);
    }
}
void CatNode::addParent(CatNode& Target)
{
    int hashvalue = std::hash<CatNode>()(Target);
    Parents.insert(std::make_pair(hashvalue, &Target));
}
void CatNode::addChild(CatNode& Target)
{
    int hashvalue = std::hash<CatNode>()(Target);
    Children.insert(std::make_pair(hashvalue, &Target));;
}
//Removes
void CatNode::removeChild(std::string Target)
{
    Children.erase(std::hash<std::string>{}(Target));
}
void CatNode::removeParent(std::string Target)
{
    Parents.erase(std::hash<std::string>{}(Target));
}
//Data Manipulation
bool const CatNode::checkCollisionList(std::vector<std::string> ListToCheck)
{
    if (ListToCheck.size() == 0)
        return true;
    if (CollisionParent.size() == 0)
        return false;
    int slot = -1;
    for (int i = 0; i < CollisionParent.size(); i++)
    {
        if (CollisionParent[i][0] == ListToCheck[0]&&CollisionParent[i].size()==ListToCheck.size())
        {
            slot = i;
            break;
        }
    }
    if (slot == -1)
    {
        return false;
    }
    for (int i = 1; i < ListToCheck.size(); i++)
    {
        if (ListToCheck[i] != CollisionParent[slot][i])
            return false;
    }
    return true;
}

//
void CatNode::print(std::ostream& out)
{
	out << *this->word<<" "<<title;
}
void CatNode::PrintCollisions()
{
    for (int i = 0; i < CollisionParent.size(); i++)
    {
        std::cout <<std::setw(10)<< "List " << i << ":" << std::endl;
        std::cout << std::setw(20);
        for (int j = 0; j < CollisionParent[i].size(); j++)
        {
            std::cout << CollisionParent[i][j];
            if (CollisionParent[i].size() != (j + 1))
                std::cout << " -> ";
        }
        std::cout << std::endl;
    }
}


std::unordered_map<int, CatNode*>::iterator CatNode::getChildernBegin(){
    return Children.begin();
}
std::unordered_map<int, CatNode*>::iterator CatNode::getChildernEnd() {
    return Children.end();
}
std::unordered_map<int, CatNode*>::iterator CatNode::getParentsBegin(){
    return Parents.begin();
}
std::unordered_map<int, CatNode*>::iterator CatNode::getParentsEnd() {
    return Parents.end();
}
const int CatNode::getNumChildren() {
    return Children.size();
}
const int CatNode::getNumParents() {
    return Parents.size();
}