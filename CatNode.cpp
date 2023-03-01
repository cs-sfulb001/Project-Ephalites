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
int const CatNode::getNumChildren()
{
	return Children.size();
}
int const CatNode::getNumParents()
{
	return Parents.size();
}
CatNode* const CatNode::getChild(int slot)
{
	return Children[slot];
}
CatNode* const CatNode::getParent(int slot)
{
	return Parents[slot];
}
bool const CatNode::getIsCollision()
{
	return isCollision;
}
bool const CatNode::isParent(std::string target)
{
    for (int i = 0; i < Parents.size(); i++)
    {
        if (Parents[i]->getWord() == target)
        {
            return true;
        }
    }
    return false;
}

bool const CatNode::isParent(CatNode& target)
{
    for (int i = 0; i < Parents.size(); i++)
    {
        if (Parents[i] == &target)
            return true;
    }
    return false;
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
    return Parents.size() != 0;
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
    Parents.push_back(&Target);
}
void CatNode::addChild(CatNode& Target)
{
    Children.push_back(&Target);
}
//Removes
void CatNode::removeChild(std::string Target)
{
    for (int i = 0; i < Children.size(); i++)
    {
        if (Children[i]->getWord() == Target)
        {
            Children.erase(Children.begin() + i);
            return;
        }
    }
}
void CatNode::removeParent(std::string Target)
{
    for (int i = 0; i < Parents.size(); i++)
    {
        if (Parents[i]->getWord() == Target)
        {
            Parents.erase(Parents.begin() + i);
            return;
        }
    }
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