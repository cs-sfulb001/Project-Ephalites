#include <fstream>
#include <string>
#include <iostream>
#include "UIHelper.h"
#include "TreeControl.h"
#include "Choices.h"
#include "StringManip.h"

//May benfit from reorganizing in to 2 or more classes
//Work has beend done to remove compile errors but the logic still needs alot of work
/*
Destruction of CatNodes will likely be best handled by BaseNodeExt because a CatNode will not exist without BaseNodeExt in the common case
ConnectCTree and Load CTree needs to beable to handle cases involving a lack of CTEqiv and how to create it.
suggest a new function with the express porpose of the transformation when needed.
*/
/*
Description:
    This Class is designed to build the LanguageTree and CategoryTree. In the build process of the Category Tree, it watches the memory usage while it builds the tree and has a method to building the tree.
    This method may need to be changed later due to the fact it loads all parents of a node if the node is already loaded regardless of memory usage. This Class also allows and programs the interactions between
    the category tree and language tree. A Category Node can not exist without a corisponding Language Node.
*/
TreeControl::TreeControl() :TreeControl(0, 2) {}
TreeControl::TreeControl(int minTU, double maxRAM)
{
    minTimesUsed = minTU;
    RAM.desiredRAMUsage(maxRAM);
}
//Some loaded lines are being sent to CTStorage
void TreeControl::buildTrees()
{
    std::cout << "Loading LTree" << std::endl;
    LTree.LoadTree();
    std::cout << "Building Trees" << std::endl;
    LoadCTreeFile();
}
void TreeControl::LoadCTreeFile()//Might be causing data lost and data duplication. Some how data is ending up in both RAM and file. Note no check is in the saving process for repeated information.
{
    std::ifstream in;
    std::ofstream out;
    in.open("CategoryTree.txt");
    out.open("CategoryTreeStorage.txt");
    std::string Line;
    while (std::getline(in, Line))
    {
        std::vector<std::string> brokenLine = SplitStringBySpaceOnly(Line);
        int added = 0;
        //std::cout << "Is it in the for loop?" << std::endl;
        for (int i = 1; i < (brokenLine.size() + 1) / 3; i++)
        {
            //std::cout << "Loop #"<<i << std::endl;
            if (LTree.findWord(brokenLine[i * 3]) != nullptr)//If it is in the LTree
            {

                //std::cout << "IF?" << std::endl;
                added++;
                CatNode* parent = nullptr;
                if (i == 1)
                    parent = CTree.getStart();
                else
                {
                    BaseNode * LParent = nullptr;
                    LParent = LTree.findWord(brokenLine[(i - 1) * 3]);

                    if (LParent == nullptr || LParent->getCTEqiv() == nullptr)
                        parent = LoadParent(brokenLine, i - 1);
                    else
                        parent = LParent->getCTEqiv();
                }
                std::vector<std::string> CPP;
                for (int j = 1; j < i; j++)
                {
                    if (LTree.findWord(brokenLine[j * 3])->getCTEqiv()->getIsCollision())
                        CPP.push_back(brokenLine[(j - 1) * 3]);
                }
                CatNode* child = LTree.findWord(brokenLine[i * 3])->getCTEqiv();
                if (!child->isParent(*parent) || !child->checkCollisionList(CPP))
                {
                    if (i == 1)
                    {
                        CTree.ConnectNodes(LTree.findWord(brokenLine[i * 3])->getCTEqiv(), CTree.getStart(), &CPP);
                    }
                    else if (i != 1)
                    {
                        CTree.ConnectNodes(LTree.findWord(brokenLine[i * 3])->getCTEqiv(), LTree.findWord(brokenLine[(i - 1) * 3])->getCTEqiv(), &CPP);
                    }
                }
            }
            else if (LTree.findWord(brokenLine[i * 3]) == nullptr && !RAM.aboveTargetUsage() && LTree.TimesUsedFromFile(brokenLine[i * 3]) >= minTimesUsed)//If it is not in the LTree and there is RAM for it and it meets the minTimesUsed condition
            {

                //std::cout << "Else IF?" << std::endl;
                added++;
                CatNode* parent = nullptr;
                if (i == 1)
                {
                    parent = CTree.getStart();
                }
                else
                {
                    BaseNode* LParent = nullptr;
                    //std::cout << "101" << std::endl;
                    LParent = LTree.findWord(brokenLine[(i - 1) * 3]);
                    //std::cout << "103" << std::endl;
                    if (LParent == nullptr || LParent->getCTEqiv() == nullptr)
                    {
                        //std::cout << "If?" << std::endl;
                        parent = LoadParent(brokenLine, i - 1);
                    }
                    else
                    {
                        //std::cout << "Else?" << std::endl;
                        parent = LParent->getCTEqiv();
                    }
                }
                std::vector<std::string> CPP;
                //std::cout << "This loop?" << std::endl;
                for (int j = 1; j < i; j++)
                {
                    if (LTree.findWord(brokenLine[j * 3])->getCTEqiv()->getIsCollision())
                        CPP.push_back(brokenLine[(j - 1) * 3]);
                }
                LTree.LoadWord(brokenLine[i * 3]);
                CatNode* child = morphBaseNode(LTree.findWord(brokenLine[i * 3]))->getCTEqiv();
                CTree.ConnectNodes(child, parent, &CPP);
                //std::cout << "Nope"<<std::endl;
            }
            //std::cout << "Does it make it to the end of the loop?" << std::endl;
        }
        std::cout << "Escaped For loop" << std::endl;
        if (added != ((brokenLine.size() + 1) / 3)-1)
            out << Line << std::endl;
    }
}
CatNode* TreeControl::LoadParent(std::vector<std::string> List, int target)
{
    //std::cout << "This function?" << std::endl;
    //std::cout << "Target" << target << "\n" << (List.size() + 1) / 3 << std::endl;
    CatNode* parent = nullptr;
    if (target == 1)//After IF Else
    {
        parent = CTree.getStart();
    }
    else
    {
        BaseNode* LParent = nullptr;
        LParent = LTree.findWord(List[(target - 1) * 3]);
        if (LParent == nullptr || LParent->getCTEqiv() == nullptr)
        {
            std::cout << "This? 146" << std::endl;
            parent = LoadParent(List, target - 1);
        }
        else
        {
            std::cout << "This? 151" << std::endl;
            parent = LParent->getCTEqiv();
        }
    }
    std::vector<std::string> CPP;
    std::cout << "In this loop?" << std::endl;
    std::cout << "Current target: " << target << " " << List[target * 3] << std::endl;
    CTree.PrintLoadedTree();
    LTree.PrintLoadedTree();
    for (int i = 1; i < target; i++)
    {
        std::cout << List[i*3] << " at i: " << i << std::endl;
        if (LTree.findWord(List[i * 3])->getCTEqiv()->getIsCollision())//Read Access violation due to vechile being removed from langauge tree
            CPP.push_back(List[(i - 1) * 3]);
    }
    std::cout << "168" << std::endl;
    LTree.LoadWord(List[target * 3]);//Problem Occurring
    std::cout << "170" << std::endl;
    BaseNode* child = LTree.findWord(List[target * 3]);
    std::cout << "172" << std::endl;
    child = morphBaseNode(child);
    LTree.PrintLoadedTree();
    std::cout << "174" << std::endl;
    CTree.ConnectNodes(child->getCTEqiv(), parent, &CPP);
    LTree.PrintLoadedTree();
    std::cout << "Leaving function" << std::endl;
    return child->getCTEqiv();
}
BaseNodeExt* TreeControl::morphBaseNode(BaseNode* oldNode)
{
    BaseNodeExt* morphedNode = nullptr;
    morphedNode = new BaseNodeExt(oldNode->getWord(), oldNode->getTitle(), oldNode->getTimesUsed());
    BaseNode* Parent = oldNode->getParent();
    std::cout << "Num Children " << Parent->getNumChildren() << std::endl;
    morphedNode->setParent(*Parent);
    Parent->removeChild(oldNode->getWord());
    Parent->addChild(*morphedNode);
    for (int i = 0; i < oldNode->getNumChildren(); i++)
    {
        //Should not be needed
        oldNode->getChild(0)->setParent(*morphedNode);
        morphedNode->addChild(*(oldNode->getChild(0)));
        oldNode->removeChild(oldNode->getChild(0)->getWord());
    }
    oldNode->removeParent();
    delete oldNode;
    std::cout << "Num Children " << Parent->getNumChildren() << std::endl;
    return morphedNode;
}
void TreeControl::SaveTrees()
{
    LTree.SaveTree();
    CTree.SaveTree();//lead to data lost
}
void TreeControl::PrintTrees()
{
    header("Loaded Language Tree", 80, '_' );
    LTree.PrintLoadedTree();
    waitTillEnter();
    header("Loaded Category Tree", 80, '_');
    CTree.PrintLoadedTree();
    waitTillEnter();
    header("Unloaded Language Tree", 80, '_');
    LTree.PrintUnloadedTree();
    waitTillEnter();
    header("Unloaded Category Tree", 80, '_');
    CTree.PrintUnloadedTree();
    waitTillEnter();
}
void TreeControl::printCollisions()
{
    header("Collisions", 80, '_');
    BaseNode* Begining = LTree.getStart();
    auto children = Begining->getChildernBegin();
    auto end = Begining->getChildernEnd();
    while(children!=end)
    {
        printCollisions(children->second);
        children++;
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
    auto children = target->getChildernBegin();
    auto end = target->getChildernEnd();
    while (children != end)
    {
        printCollisions(children->second);
        children++;
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
                if (!choiceYN("Would you like to choose a diffrent node?"))
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
                if (!choiceYN("Would you like to choose a diffrent node?"))
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
                if (!choiceYN("Would you like to choose a diffrent node?"))
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
            auto Grandparent = parent->getParentsBegin();
            auto end = parent->getParentsEnd();
            int i = 0;
            std::vector<std::string> options;
            while(Grandparent!=end)
            {
                if (Grandparent->second->checkCollisionList(CollisionList))//potental error in future
                {
                    std::cout << i << ". " << Grandparent->second->getWord() << std::endl;
                    options.push_back(Grandparent->second->getWord());
                }
                Grandparent++;
            }
            int slot = -1;
            std::cin >> slot;
            if (slot != -1 && parent->getParent(options[slot])->checkCollisionList(CollisionList))
            {
                CollisionList.push_back(parent->getParent(options[slot])->getWord());
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
void TreeControl::PrintMemoryUsage()
{
    char prefix = 'G';
    while (RAM.getRAMUsage(prefix) < 1) {
        if (prefix == 'G')
            prefix = 'M';
        else if (prefix == 'M')
            prefix = 'K';
        else
        {
            prefix = NULL;
            break;
        }
    }
    std::cout << "Current Memory Usage: " << RAM.getRAMUsage(prefix)<<prefix<< "B out of " << RAM.desiredRAMUsage() << "GB" << std::endl;
}
bool TreeControl::inLTree(std::string word) {
    return LTree.findWord(word) != nullptr || LTree.inFile(word);
}