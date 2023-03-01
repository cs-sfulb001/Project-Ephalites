#include <fstream>
#include <string>
#include <iostream>
#include "UIHelper.h"
#include "TreeControl.h"
#include "Choices.h"

//May benfit from reorganizing in to 2 or more classes
//Work has beend done to remove compile errors but the logic still needs alot of work
/*
Destruction of CatNodes will likely be best handled by BaseNodeExt because a CatNode will not exist without BaseNodeExt in the common case
ConnectCTree and Load CTree needs to beable to handle cases involving a lack of CTEqiv and how to create it.
suggest a new function with the express porpose of the transformation when needed.
*/
TreeControl::TreeControl() :TreeControl(0, 2){}
TreeControl::TreeControl(int minTU, double maxRAM)
{
    LTree.setMinUses(minTU);
    RAM.desiredRAMUsage(maxRAM);
}
void TreeControl::buildTrees()
{
    std::cout << "Building LTree" << std::endl;
    LTree.LoadTree();
    std::cout << "Building CTree" << std::endl;
    LoadCTreeFile();
}
void TreeControl::LoadCTreeFile()
{
    std::ifstream in;
    in.open("CategoryTree.txt");
    CatNode* parentAddress = nullptr;
    std::string word = "";
    bool title = 0;
    std::string Misc = "";
    std::vector<std::string> CollisionParentList;
    while (true)
    {
        if (word == "")
        {
            in >> word >> title >> Misc;
        }
        else
        {
            in >> title >> Misc;
        }
        if (word == "")
        {
            break;
        }
        CatNode* wordAddress = nullptr;
        if (word == "Category")
        {
            wordAddress = CTree.getStart();
        }
        else if(LTree.findWord(word)!=nullptr)
        {
            BaseNode* target = LTree.findWord(word);
            if (target->getCTEqiv() != nullptr)
            {
                wordAddress = target->getCTEqiv();
            }
            else
            {
                target=morphBaseNode(target);
                wordAddress = target->getCTEqiv();
            }
        }
        if (parentAddress != nullptr && wordAddress != nullptr)
        {
            CTree.ConnectNodes(wordAddress, parentAddress, &CollisionParentList);
        }
        if (Misc == "->")
        {
            parentAddress = wordAddress;
            Misc = "";
            title = 0;
            word = "";
        }
        else
        {
            parentAddress = nullptr;
            word = Misc;
            Misc = "";
            title = 0;
            if (CollisionParentList.size() != 0)
                CollisionParentList.clear();
        }
    }
}
BaseNodeExt* TreeControl::morphBaseNode(BaseNode* oldNode)
{
    BaseNodeExt* morphedNode = nullptr;
    morphedNode = new BaseNodeExt(oldNode->getWord(), oldNode->getTitle(), oldNode->getTimesUsed());
    BaseNode* Parent = oldNode->getParent();
    morphedNode->setParent(*Parent);
    Parent->removeChild(oldNode->getWord());
    Parent->addChild(*morphedNode);
    for (int i = 0; i < oldNode->getNumChildren(); i++)
    {
        oldNode->getChild(0)->setParent(*morphedNode);
        morphedNode->addChild(*(oldNode->getChild(0)));
        oldNode->removeChild(oldNode->getChild(0)->getWord());
    }
    oldNode->removeParent();
    delete oldNode;
    return morphedNode;
}
void TreeControl::SaveTrees()
{
    LTree.SaveTree();
    CTree.SaveTree();
}
void TreeControl::PrintTrees()
{
    header("Language Tree", 80, '_' );
    LTree.PrintTree();
    waitTillEnter();
    header("Category Tree", 80, '_');
    CTree.PrintTree();
    waitTillEnter();
}
void TreeControl::printCollisions()
{
    header("Collisions", 80, '_');
    BaseNode* Begining = LTree.getStart();
    for (int i = 0; i < Begining->getNumChildren(); i++)
    {
        printCollisions(Begining->getChild(i));
    }
    waitTillEnter();
}
void TreeControl::printCollisions(BaseNode* target)
{
    if (target->getCTEqiv() != nullptr)
    {
        std::cout << *target << std::endl;
        target->getCTEqiv()->PrintCollisions();
    }
    for (int i = 0; i < target->getNumChildren(); i++)
    {
        printCollisions(target->getChild(i));
    }
}
void TreeControl::AddToLTree()
{
    while (true)
    {
        std::string word = choiceWord("What word would you like to add?");
        if (LTree.findWord(word) != nullptr)
        {
            if (!choiceYN("That word is already in the Tree\nWould you still like to add to the tree?"))
                return;
        }
        else
        {
            AddToLTree(word);
            return;
        }
    }
}
void TreeControl::AddToLTree(std::string word)
{
    while (true)
    {
        std::string Pword = choiceWord("What is the group you would like to add to");
        if (LTree.findTitle(Pword) == nullptr)
        {
            if (choiceYN("Would you like to make it a group?"))
            {
                LTree.NewTitleNode(Pword);
                LTree.NewNode(word, Pword, 0);
                return;
            }
            else
            {
                if (!choiceYN("Would you like to pick a new group?"))
                {
                    return;
                }
            }
        }
        else
        {
            LTree.NewNode(word, Pword,0);
            return;
        }

    }
}
/*
start of input of a new Category Tree connection
Handles finding the desired child node
*/
void TreeControl::ConnectInCTree()
{
    while (true)
    {
        std::string word = choiceWord("What word will be the child?");
        if (LTree.findWord(word) == nullptr)
        {
            if (choiceYN("That word does not exist in Language Tree. Would you like to add it?"))
            {
                AddToLTree(word);
                ConnectInCTree(LTree.findWord(word));
                return;
            }
            else
            {
                if (!choiceDifNode())
                    return;
            }
        }
        else
        {
            ConnectInCTree(LTree.findWord(word));
            return;
        }
    }
}
/*
Handles finding the desired parent node
*/
void TreeControl::ConnectInCTree(BaseNode* child)
{
    while (true)
    {
        std::string word = choiceWord("What word will be the parent?");
        BaseNode* Target = LTree.findWord(word);
        if (Target == nullptr && word != "Category")
        {
            if (choiceYN("That word does not exist in Language Tree. Would you like to add it?"))
            {
                AddToLTree(word);
                Target = LTree.findWord(word);
                std::cout << "You now need to connect it in Category Tree" << std::endl;
                ConnectInCTree(Target);
                if (child->getCTEqiv() == nullptr)
                {
                    morphBaseNode(child);
                }
                (*child)++;
                (*Target)++;
                ConnectInCTree(child->getCTEqiv(), Target->getCTEqiv());
                return;
            }
            else
            {
                if (!choiceDifNode())
                    return;
            }
        }
        else if (word != "Category" && Target->getCTEqiv() == nullptr)
        {
            if (choiceYN("The node is not in Category Tree. Would you like to add it?"))
            {
                ConnectInCTree(Target);
                if (child->getCTEqiv() == nullptr)
                {
                    morphBaseNode(child);
                }
                (*child)++;
                (*Target)++;
                ConnectInCTree(child->getCTEqiv(), Target->getCTEqiv());
                return;
            }
            else
            {
                if (!choiceDifNode())
                    return;
            }
        }
        else
        {
            CatNode* parent = nullptr;
            if (word == "Category")
            {
                parent = CTree.getStart();
            }
            else
            {
                if (Target->getCTEqiv() == nullptr)
                {
                    Target= morphBaseNode(Target);
                }
                parent = LTree.findWord(word)->getCTEqiv();
                (* Target)++;
            }
            if (child->getCTEqiv() == nullptr)
                child = morphBaseNode(child);
            (*child)++;
            ConnectInCTree(child->getCTEqiv(), parent);
            return;
        }
    }
}
void TreeControl::ConnectInCTree(CatNode* child, CatNode* parent)
{
    std::vector<std::string> CollisionList;
    if (parent->getAnyCollisionParents())
    {
        while (true)
        {
            std::cout << "What collision parent list should the child have" << std::endl;
            parent->PrintCollisions();
            int list = -1;
            std::cin >> list;
            if (list >= 0)
            {
                CollisionList = parent->getCollisionList(list);
                break;
            }
            else
                std::cout << "That is not an option" << std::endl;
        }
    }
    if (parent->getIsCollision())
    {
        while (true)
        {
            std::cout << "Which parent of " << parent->getWord() << " will be " << child->getWord() << " collision parent " << std::endl;
            for (int i = 0; i < parent->getNumParents(); i++)
            {
                if (parent->getParent(i)->checkCollisionList(CollisionList))//potental error in future
                    std::cout << i << ". " << parent->getParent(i)->getWord() << std::endl;
            }
            int slot = -1;
            std::cin >> slot;
            if (slot != -1 && parent->getParent(slot)->checkCollisionList(CollisionList))
            {
                CollisionList.push_back(parent->getParent(slot)->getWord());
                break;
            }
            else
                std::cout << "That is not an option" << std::endl;
        }

    }
    CTree.ConnectNodes(child, parent, &CollisionList);
}
LanguageTree* TreeControl::GetLT()
{
    return &LTree;
}