#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include "CategoryTree.h"

CategoryTree::CategoryTree()
{
    start = new CatNode(startWord, StartTimesUsed);
}
CategoryTree::~CategoryTree()
{
    delete start;
}
//Problems with save file is due to loading issues
void CategoryTree::SaveTree()
{
    std::vector<std::string> CollisionParentList;
    std::vector<CatNode*> Path;
    Path.push_back(start);
    std::ofstream out;
    out.open("CategoryTree.txt");
    auto CurrentNode = start->getChildernBegin();
    auto end = start->getChildernEnd();
    while(CurrentNode!=end)
    {
        SaveTree(CurrentNode->second, CollisionParentList, Path, out);
        CurrentNode++;
    }
    std::ifstream storage;
    storage.open("CategoryTreeStorage.txt");
    std::string line;
    while (std::getline(storage, line))
    {
        out << line << std::endl;
    }
    out.close();
    storage.close();
    remove("CategoryTreeStorage.txt");
}
void CategoryTree::SaveTree(CatNode* Target, std::vector<std::string> CollisionParentList, std::vector<CatNode*> Path, std::ofstream& out)
{
    Path.push_back(Target);
    bool printed = 0;
    auto CurrentNode = Target->getChildernBegin();
    auto end = Target->getChildernEnd();
    while (CurrentNode!=end)
    {
        bool added = 0;
        if (CurrentNode->second->checkCollisionList(CollisionParentList))
        {
            if (CurrentNode->second->getIsCollision())
            {
                CollisionParentList.push_back(Target->getWord());
                added = 1;
            }
            SaveTree(CurrentNode->second, CollisionParentList, Path, out);
            if (added)
            {
                CollisionParentList[CollisionParentList.size() - 1].erase();
            }
            printed = 1;
        }
        CurrentNode++;
    }
    if (!printed)
    {
        for (size_t i = 0; i < Path.size(); i++)
        {
            if (i != Path.size() - 1)
            {
                Path[i]->print(out);
                out << " -> ";
            }
            else
            {
                Path[i]->print(out);
                out  << std::endl;
            }
        }
    }
}
void CategoryTree::ConnectNodes(CatNode* Child, CatNode* Parent, std::vector<std::string>* CollisionParents)//How back fill is handled is wrong
{
    if (CheckCollisionCreation(Child, Parent->getWord()) && !Child->getIsCollision())
    {
        Child->setCollisionStatus(1);
        /*
        CollisionBackFill should not take in the collision list that is currently being modified. Instead it needs to modify the previous CPP that was contained.
        For all CPP's that is shared by the child node and its children?
        */
        CollisionBackFill(Child, *CollisionParents);
    }
    Child->AddCollisionParent(*CollisionParents);
    if (Child->getIsCollision())
    {
        CollisionParents->push_back(Parent->getWord());
    }
    if (Child->isParent(*Parent))
    {
        return;
    }
    Child->addParent(*Parent);
    Parent->addChild(*Child);
}
bool CategoryTree::CheckCollisionCreation(CatNode* Child, std::string Parent)
{
    return Child->getNumParents() > 0 && !Child->isParent(Parent);
}
void CategoryTree::CollisionBackFill(CatNode* CollisionPoint, std::vector<std::string> CollisionParents)
{
    int newCollision = CollisionParents.size();
    //std::cout << CollisionPoint->getNumParents() << std::endl;
    CollisionParents.push_back(CollisionPoint->getParentsBegin()->second->getWord());
    auto CollisionChildrend = CollisionPoint->getChildernBegin();
    auto end = CollisionPoint->getChildernEnd();
    while (CollisionChildrend != end) {//Simplified with use of CollisionFill. Need to double check it holds the same functionality.
        CollisionFill(CollisionChildrend->second, newCollision, CollisionPoint, CollisionParents);
        CollisionChildrend++;
    }
}
void CategoryTree::CollisionFill(CatNode* Target, int newCollisionPoint, CatNode* TargetParent, std::vector<std::string> CollisionParents)
{
    //std::cout << "Is it at the start" << std::endl;
    Target->AddCollisionParent(CollisionParents, newCollisionPoint);
    //std::cout << "or before the if?" << std::endl;
    if (Target->getIsCollision())
    {
        CollisionParents.push_back(TargetParent->getWord());
    }
    //std::cout << "Or with in the for loop" << std::endl;
    auto TargetChildrend = Target->getChildernBegin();
    auto end = Target->getChildernEnd();
    while (TargetChildrend != end) {
        //std::cout << "CollisionFill happening" << std::endl;
        CollisionFill(TargetChildrend->second, newCollisionPoint, Target, CollisionParents);
        //std::cout << "CollisionFill Done" << std::endl;
        TargetChildrend++;
    }
    
}
CatNode* CategoryTree::getStart()
{
    return start;
}
void CategoryTree::PrintLoadedTree()
{
    if (start->getNumChildren() == 0)
    {
        PrintNode(start);
        return;
    }
    auto children = start->getChildernBegin();
    auto end = start->getChildernEnd();
    while(children!=end)
    {
        std::cout << std::setw(10);
        start->print(std::cout);
        std::cout<< " -> ";
        std::vector<std::string> CollisionParentList;
        PrintLoadedTree(children->second, CollisionParentList, 1);
        children++;
    }
}
void CategoryTree::PrintLoadedTree(CatNode* Target, std::vector<std::string> CollisionParentList, int depth)
{
    if (Target->getNumChildren() == 0)
    {
        PrintNode(Target);
        return;
    }
    bool printed = 0;

    auto children = Target->getChildernBegin();
    auto end = Target->getChildernEnd();
    while(children!=end)
    {
        bool added = 0;
        if (children->second->checkCollisionList(CollisionParentList))
        {
            int a = 10;
            if (printed)
            {
                a = 10 * (depth + 1) + 6 * depth;
            }
            if (children->second->getIsCollision())
            {
                CollisionParentList.push_back(Target->getWord());
                added = 1;
            }
            std::cout << std::setw(a);
            Target->print(std::cout);
            std::cout<< " -> ";
            PrintLoadedTree(children->second, CollisionParentList, depth + 1);
            if (added)
            {
                CollisionParentList[CollisionParentList.size() - 1].erase();
            }
            printed = 1;
            
        }
        children++;
    }
    if (!printed)
        PrintNode(Target);
}
void CategoryTree::PrintUnloadedTree()
{
    std::ifstream storage;
    storage.open("CategoryTreeStorage.txt");
    std::string line;
    while (std::getline(storage, line))
    {
        std::cout << line << std::endl;
    }
    storage.close();
}
void CategoryTree::PrintNode(CatNode* Target)
{
    std::cout << std::setw(10);
    Target->print(std::cout);
    std::cout<< std::endl;
}
