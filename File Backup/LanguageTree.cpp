#include <iomanip>
#include <fstream>
#include "LanguageTree.h"

//Constructors
LanguageTree::LanguageTree()
{
    start = new BaseNode("Language", 1);
    noun = nullptr;
    pronoun = nullptr;
    propernoun = nullptr;
    verb = nullptr;
    minUses = 0;
}
//Destructors

LanguageTree::~LanguageTree()
{
    noun = nullptr;
    pronoun = nullptr;
    propernoun = nullptr;
    verb = nullptr;
    //std::cout << *start << " has " << start->getNumChildren() << " children " << std::endl;
    while (start->getNumChildren()!=0)
    {
        //std::cout << *(start->getChild(0)) << " has " << start->getChild(0)->getNumChildren() << " children " << std::endl;
        while (start->getChild(0)->getNumChildren()!=0)
        {
            //std::cout << "Deleting: " << *(start->getChild(0)->getChild(0)) << std::endl;
            delete (start->getChild(0)->getChild(0)); 
        }
        delete start->getChild(0);
    }
    delete start;
}
//Print functions
void LanguageTree::PrintTree()
{
    if (start->getNumChildren() == 0)
        std::cout << std::setw(10) << start->getWord() << " " << start->getTitle() << std::endl;
    for (int i = 0; i < start->getNumChildren(); i++)
    {
        std::cout << std::setw(10) << *start << " -> ";
        PrintTree(start->getChild(i), 1);
        std::cout << std::endl;
    }
}
void LanguageTree::PrintTree(BaseNode* target, int depth)
{
    int num = target->getNumChildren();
    if (num == 0)
    {
        std::cout << std::setw(10) << *target;
    }
    else
    {
        for (int i = 0; i < num; i++)
        {
            if (i == 0)
            {
                std::cout << std::setw(10) << *target << " -> ";
                PrintTree(target->getChild(i), depth + 1);
                std::cout << std::endl;
            }
            else
            {
                int a = 12 * (depth + 1) + 4 * depth;
                std::cout << std::setw(a) << *target << " -> ";
                PrintTree(target->getChild(i), depth + 1);
                std::cout << std::endl;
            }
        }
    }
}
//Load from file
void LanguageTree::LoadTree()
{
    std::ifstream in;
    in.open("LanguageTree.txt");
    std::string parent = "";
    std::string word = "";
    bool title = 0;
    int times = 0;
    std::string Misc = "";
    while (true)
    {
        if (word == "")
        {
            in >> word >> title >> times>> Misc;
        }
        else
        {
            in >> title >> times>> Misc;
        }
        //std::cout<<parent<<" -> "<<word<<" "<<title<<" "<<times<<Misc<<std::endl;
        if (word == "")
        {
            break;
        }
        if (parent != "")
        {
            BaseNode* WordAdress = findWord(word);
            if (title && WordAdress == nullptr)
            {
                NewTitleNode(word);
            }
            else if (WordAdress == nullptr)
            {
                //std::cout<<"Loading "<<word<<std::endl;
                NewNode(word, parent, times);
            }
        }
        if (Misc != "->")
        {
            parent = "";
            word = Misc;
            Misc = "";
        }
        else
        {
            parent = word;
            word = "";
            Misc = "";
        }
        //std::cout<<"Finished loading this word"<<std::endl;
    }
}
//Save to file
void LanguageTree::SaveTree()
{
    std::ofstream out;
    out.open("LanguageTree.txt");
    if (start->getNumChildren() == 0)
    {
        out << *start << std::endl;
        return;
    }
    for (int i = 0; i < start->getNumChildren(); i++)
    {
        if (start->getChild(i)->getNumChildren() == 0)
        {
            out << *start << " -> " << *start->getChild(i) << std::endl;
        }
        for (int j = 0; j < start->getChild(i)->getNumChildren(); j++)
        {
            out << *start << " -> " << *start->getChild(i) << " -> " << *start->getChild(i)->getChild(j) << std::endl;
        }
    }
}
//Node Managers
BaseNode* LanguageTree::getStart()
{
    return start;
}
    //Find Node Functions
BaseNode* LanguageTree::findWord(std::string targetWord)
{
    return findWord(targetWord, start);
}
BaseNode* LanguageTree::findWord(std::string targetWord, std::string parent)
{
    if (parent == "Noun" && noun != nullptr)
    {
        return findWord(targetWord, noun);
    }
    if (parent == "Pronoun" && pronoun != nullptr)
    {
        return findWord(targetWord, pronoun);
    }/*
    if(parent=="Propernoun"&&propernoun!=nullptr);
    {
        std::cout << "In propernoun" << std::endl;
        return findWord(targetWord, propernoun);
    }*/
    if (parent == "Verb" && verb != nullptr)
    {
        return findWord(targetWord, verb);
    }
    return findWord(targetWord, start);
}
BaseNode* LanguageTree::findWord(std::string targetWord, BaseNode* currentNode)//Caindent for multi threading?
{
    //std::cout<<"Current Node address"<<currentNode << std::endl;
    //std::cout<<"Crashing in FindWord?"<<std::endl;
    if (currentNode->getWord() == targetWord)
        return currentNode;
    for (int i = 0; i < currentNode->getNumChildren(); i++)
    {
        //std::cout<<"In for loop?"<<std::endl;
        BaseNode* NodeReturn = nullptr;
        NodeReturn = findWord(targetWord, currentNode->getChild(i));
        if (NodeReturn != nullptr)
            return NodeReturn;
    }
    return nullptr;
}

//add node functions
void LanguageTree::NewNode(std::string child, std::string parent, int times)
{
    if (findWord(child, parent) != nullptr)//Checks if the node already exist using the parent to refine the search
    {
        return;
    }
    if (findWord(parent) == nullptr)
    {
        NewTitleNode(parent);
    }
    BaseNode* parentNode = findWord(parent);
    parentNode->addChild(child, 0, times);
}
void LanguageTree::NewTitleNode(std::string child)
{
    if (findWord(child) != nullptr)
    {
        std::cout << "Node already exists" << std::endl;
        return;
    }
    BaseNode* newNode = new BaseNode(child, 1,*start);

    if (child == "Noun")
    {
        noun = newNode;
        start->addChild(*newNode);
        return;
    }
    else if (child == "Pronoun")
    {
        pronoun = newNode;
        start->addChild(*newNode);
        return;
    }
    else if (child == "Propernoun")
    {
        propernoun = newNode;
        start->addChild(*newNode);
        return;
    }
    else if (child == "Verb")
    {
        verb = newNode;
        start->addChild(*newNode);
        return;
    }
    else
    {
        delete newNode;
    }
}
//Remove node functions
void LanguageTree::RemoveLangNode(std::string Target)
{
    BaseNode* Targetaddress = findWord(Target);
    if (Targetaddress == nullptr)
    {
        std::cout << "Target does not exist" << std::endl;
    }
    Targetaddress->getParent()->removeChild(Target);
    Targetaddress->removeParent();
    delete Targetaddress;
}
BaseNode* LanguageTree::findTitle(std::string Target)
{
    for (int i = 0; i < start->getNumChildren(); i++)
    {
        if (start->getChild(i)->getWord() == Target)
            return start->getChild(i);
    }
    return nullptr;
}