#include <fstream>
#include <iomanip>
#include <vector>
#include "CategoryTree.h"

CategoryTree::CategoryTree()
{
    start = new CatNode(startWord, StartTimesUsed);
}
CategoryTree::~CategoryTree()
{
    //delete start->getWordPointer();
    //delete start->getTimesUsedPointer();
    delete start;
}

void CategoryTree::SaveTree()
{
    std::vector<std::string> CollisionParentList;
    std::vector<CatNode*> Path;
    Path.push_back(start);
    std::ofstream out;
    out.open("CategoryTree.txt");
    for (int i = 0; i < start->getNumChildren(); i++)
    {
        SaveTree(start->getChild(i), CollisionParentList, Path, out);
    }
}
void CategoryTree::SaveTree(CatNode* Target, std::vector<std::string> CollisionParentList, std::vector<CatNode*> Path, std::ofstream& out)
{
    Path.push_back(Target);
    bool printed = 0;
    for (int i = 0; i < Target->getNumChildren(); i++)
    {
        bool added = 0;
        if (Target->getChild(i)->checkCollisionList(CollisionParentList))
        {
            if (Target->getChild(i)->getIsCollision())
            {
                CollisionParentList.push_back(Target->getWord());
                added = 1;
            }
            SaveTree(Target->getChild(i), CollisionParentList, Path, out);
            if (added)
            {
                CollisionParentList[CollisionParentList.size() - 1].erase();
            }
            printed = 1;
        }
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
void CategoryTree::ConnectNodes(CatNode* Child, CatNode* Parent, std::vector<std::string>* CollisionParents)
{
    if (CheckCollisionCreation(Child, Parent->getWord()) && !Child->getIsCollision())
    {
        Child->setCollisionStatus(1);
        CollisionBackFill(Child, *CollisionParents);
    }
    Child->AddCollisionParent(*CollisionParents);
    if (Child->getIsCollision())
    {
        CollisionParents->push_back(Parent->getWord());
    }
    if (Child->isParent(*Parent))
    {
        //std::std::cout<<Child->getWord()<<" and "<<Parent->getWord()<<" are already connected."<<std::std::endl;
        return;
    }
    Child->addParent(*Parent);
    Parent->addChild(*Child);
}
bool CategoryTree::CheckCollisionCreation(CatNode* Child, std::string Parent)
{
    return Child->getNumChildren() > 0 && !Child->isParent(Parent);
}
void CategoryTree::CollisionBackFill(CatNode* CollisionPoint, std::vector<std::string> CollisionParents)
{
    CatNode* Target = nullptr;
    int newCollision = CollisionParents.size();
    CollisionParents.push_back(CollisionPoint->getParent(0)->getWord());
    for (int i = 0; i < CollisionPoint->getNumChildren(); i++)
    {
        bool added = 0;
        Target = CollisionPoint->getChild(i);
        Target->AddCollisionParent(CollisionParents);
        if (Target->getIsCollision())
        {
            CollisionParents.push_back(CollisionPoint->getWord());
            added = 1;
        }
        for (int j = 0; j < Target->getNumChildren(); j++)
        {
            //std::cout << "Is it in collision fill?" << std::endl;
            CollisionFill(Target->getChild(j), newCollision, Target, CollisionParents);
            //std::cout << "or is it after?" << std::endl;
        }
        if (added)
            CollisionParents.erase(CollisionParents.end());
        //std::cout << "Loop " << i << std::endl;
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
    for (int i = 0; i < Target->getNumChildren(); i++)
    {
        CollisionFill(Target->getChild(i), newCollisionPoint, Target, CollisionParents);
    }
}
CatNode* CategoryTree::getStart()
{
    return start;
}
void CategoryTree::PrintTree()
{
    if (start->getNumChildren() == 0)
    {
        PrintNode(start);
        return;
    }
    for (int i = 0; i < start->getNumChildren(); i++)
    {
        std::cout << std::setw(10);
        start->print(std::cout);
        std::cout<< " -> ";
        std::vector<std::string> CollisionParentList;
        PrintTree(start->getChild(i), CollisionParentList, 1);
    }
}
void CategoryTree::PrintTree(CatNode* Target, std::vector<std::string> CollisionParentList, int depth)
{
    if (Target->getNumChildren() == 0)
    {
        PrintNode(Target);
        return;
    }
    bool printed = 0;
    for (int i = 0; i < Target->getNumChildren(); i++)
    {
        bool added = 0;
        if (Target->getChild(i)->checkCollisionList(CollisionParentList))
        {
            int a = 10;
            if (printed)
            {
                a = 10 * (depth + 1) + 6 * depth;
            }
            if (Target->getChild(i)->getIsCollision())
            {
                CollisionParentList.push_back(Target->getWord());
                //std::cout << "Adding " << Target->getWord() << " as a collision parent" << std::endl;
                added = 1;
            }
            std::cout << std::setw(a);
            Target->print(std::cout);
            std::cout<< " -> ";
            PrintTree(Target->getChild(i), CollisionParentList, depth + 1);
            if (added)
            {
                //std::cout << "Crashing because of erasing?" << std::endl;
                std::cout << CollisionParentList.size() << std::endl;
                //std::cout << "Removing " << CollisionParentList[CollisionParentList.size()-1] << " from the list" << std::endl;
                CollisionParentList[CollisionParentList.size() - 1].erase();//something up with collision list
                //std::cout << "Not Erasing" << std::endl;
            }
            printed = 1;
        }
    }
    if (!printed)
        PrintNode(Target);
}
void CategoryTree::PrintNode(CatNode* Target)
{
    std::cout << std::setw(10);
    Target->print(std::cout);
    std::cout<< std::endl;
}
