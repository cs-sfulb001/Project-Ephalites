#include <iomanip>
#include <fstream>
#include <string>
#include "LanguageTree.h"
#include "StringManip.h"

//Constructors
LanguageTree::LanguageTree()
{
    start = new BaseNode("Language", 1);
    noun = nullptr;
    pronoun = nullptr;
    propernoun = nullptr;
    verb = nullptr;
}
//Destructors
LanguageTree::~LanguageTree()//Greatly simplified due to Basenode handling its own children
{
    delete start;
}
//Print functions
void LanguageTree::PrintLoadedTree()
{
    if (start->getNumChildren() == 0)
        std::cout << std::setw(10) << start->getWord() << " " << start->getTitle() << std::endl;
    else
    {
        std::unordered_map<int, BaseNode*>::iterator currentChild = start->getChildernBegin();
        auto end = start->getChildernEnd();
        while (currentChild != end) {
            std::cout << std::setw(10) << *start << " -> ";
            PrintLoadedTree(currentChild->second, 1);
            std::cout << std::endl;
            currentChild++;
        }
    }
}
//Printing has uncovered a nullptr that has been added to the language tree between the 1st and second addition
void LanguageTree::PrintLoadedTree(BaseNode* target, int depth)
{
    int num = target->getNumChildren();
    if (num == 0)
    {
        std::cout << std::setw(10) << *target;
    }
    else
    {
        std::unordered_map<int, BaseNode*>::iterator currentChild = target->getChildernBegin();
        auto end = target->getChildernEnd();
        std::cout << std::setw(10) << *target << " -> ";
        PrintLoadedTree(currentChild->second, depth + 1);
        std::cout << std::endl;
        currentChild++;
        while (currentChild != end) {
            int a = 12 * (depth + 1) + 4 * depth;
            std::cout << std::setw(a) << *target << " -> ";
            //std::cout << currentChild->first << std::endl;
            PrintLoadedTree(currentChild->second, depth + 1);
            std::cout << std::endl;
            currentChild++;
        }
    }
}

void LanguageTree::PrintUnloadedTree()
{
    std::ifstream storage;
    storage.open("LTreeStorage.txt");
    std::string line;
    while (std::getline(storage, line))
    {
        std::cout << line << std::endl;
    }
    storage.close();
}
//Load from file
/* Old version (does not work with Memory mangment)
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
    }
}
*/
void LanguageTree::LoadTree()
{
    std::ifstream in;
    in.open("LanguageTree.txt");
    std::ofstream storage;
    storage.open("LTreeStorage.txt");
    std::string line;
    while(std::getline(in, line))
    {
        storage << line << std::endl;
    }
    in.close();
    storage.close();
}
void LanguageTree::LoadWord(std::string word)
{
    //std::cout << "Made it to Load Word" << std::endl;
    std::ifstream storage;
    storage.open("LTreeStorage.txt");
    std::ofstream buffer;
    buffer.open("LTreeBuffer.txt");
    std::string line;
    while (std::getline(storage, line))
    {
        //std::cout << "In Loop?" << std::endl;
        std::vector<std::string> brokenLine = SplitStringBySpaceOnly(line);
        int size = brokenLine.size();
        if (brokenLine[8] == word)
        {
            //std::cout << "Creating Node " << word << std::endl;
            NewNode(word, brokenLine[4], StringtoInt(brokenLine[10]));
        }
        else
        {
            buffer << line << std::endl;
        }
    }
    //std::cout << "Out of loop" << std::endl;
    buffer.close();
    std::ifstream buffer2;
    buffer2.open("LTreeBuffer.txt");
    storage.close();
    remove("LTreeStorage.txt");
    std::ofstream storage2;
    storage2.open("LTreeStorage.txt");
    while (std::getline(buffer2, line))
    {
        //std::cout << "Adding: " << line << " :to storage" << std::endl;
        storage2 << line << std::endl;
    }
    storage.close();
    buffer2.close();
    remove("LTreeBuffer.txt");
}
//Save to file
void LanguageTree::SaveTree()
{
    std::ofstream out;
    out.open("LanguageTree.txt");
    auto currentChild = start->getChildernBegin();
    auto end = start->getChildernEnd();
    while(currentChild!=end)
    {
        if (currentChild->second->getNumChildren() == 0)
        {
            out << *start << " -> " << *currentChild->second << std::endl;
        }

        auto grandChildren = currentChild->second->getChildernBegin();
        auto grandEnd = currentChild->second->getChildernEnd();
        while(grandChildren!=grandEnd)
        {
            out << *start << " -> " << *currentChild->second << " -> " << *grandChildren->second << std::endl;
            grandChildren++;
        }
        currentChild++;
    }
    std::ifstream storage;
    storage.open("LTreeStorage.txt");
    std::string line;
    //std::cout << "Getting info from storage" << std::endl;
    while (std::getline(storage, line))
    {
        //std::cout << line << std::endl;
        out << line << std::endl;
    }
    storage.close();
    remove("LTreeStorage.txt");
    out.close();
}
//Node Managers
BaseNode* LanguageTree::getStart()
{
    return start;
}
    //Find Node Functions
BaseNode* LanguageTree::findWord(std::string targetWord)
{
    //std::cout << "FindWord without Parents" << std::endl;
    auto it = start->getChildernBegin();
    auto end = start->getChildernEnd();
    //std::cout << "Loop?" << std::endl;
    while (it != end) {
        if(it->second!=nullptr)
        {
            BaseNode* result = it->second->getChild(targetWord);
            if (result != nullptr)
                return result;
        }
        //std::cout << "Is it this ++" << std::endl;
        it++;
    }
    //std::cout << "Found Nothing" << std::endl;
    return nullptr;
}
BaseNode* LanguageTree::findWord(std::string targetWord, std::string parent)
{
    //std::cout << "FindWord" << std::endl;
    //PrintLoadedTree();
    BaseNode* parentNode = start->getChild(parent);
    //PrintLoadedTree();
    //std::cout << "IF" << std::endl;
    if (parentNode != nullptr)
        return parentNode->getChild(targetWord);
    //std::cout << "Returning" << std::endl;
    return findWord(targetWord);
}
bool LanguageTree::inFile(std::string targetWord)
{
    std::ifstream storage;
    storage.open("LTreeStorage.txt");
    std::string line;
    while (std::getline(storage, line))
    {
        std::vector<std::string> brokenLine = SplitStringBySpaceOnly(line);
        if (brokenLine[8] == targetWord)
            return true;
    }
    storage.close();
    return false;
}
int LanguageTree::TimesUsedFromFile(std::string targetWord)//Crashes if file gets corupted
{
    std::ifstream storage;
    storage.open("LTreeStorage.txt");
    std::string line;
    while (std::getline(storage, line))
    {
        std::vector<std::string> brokenLine = SplitStringBySpaceOnly(line);
        if (brokenLine[8] == targetWord)
            return StringtoInt(brokenLine[10]);
    }
    storage.close();
    return -1;
}

//add node functions
void LanguageTree::NewNode(std::string child, std::string parent, int times)
{
    //std::cout << "In New Node" << std::endl;
    //std::cout << std::hash<std::string>()("Test") << std::endl;
    //findWord("Test");
    //PrintLoadedTree();//passed
    if (findWord(child, parent) != nullptr)//Checks if the node already exist using the parent to refine the search
    {
        return;
    }
    //std::cout << "Creating title node?" << std::endl;
    //PrintLoadedTree();//Caused to die before noun is made but works till car without Findword(child,Parent) is likely problem
    if (findTitle(parent) == nullptr)
    {
        //std::cout << "Could not find title " << parent << std::endl;
        NewTitleNode(parent);//Problem occuring
        //std::cout << "Test storage: " << start->getChild(parent) << std::endl;
    }
    //std::cout << "Finding Parent node?" << std::endl;
    //PrintLoadedTree();
    BaseNode* parentNode = findTitle(parent);
    //std::cout << "**************************" << std::endl;
    //PrintLoadedTree();
    //std::cout <<"Parent node is "<< parentNode << *parentNode << std::endl;
    //std::cout << "Adding Child?" << std::endl;
    parentNode->addChild(child, 0, times);
    //PrintLoadedTree();
    //std::cout << "**************************" << std::endl;

    //std::cout << "Leaving New Node" << std::endl;
}
void LanguageTree::NewTitleNode(std::string child)
{
    if (findWord(child) != nullptr)
    {
        //std::cout << "Node already exists" << std::endl;
        return;
    }
    BaseNode* newNode = new BaseNode(child, 1,*start);
    //std::cout << "Adding node " << *newNode << " at address " << newNode << std::endl;
    start->addChild(*newNode);
}
//Remove node functions
void LanguageTree::RemoveLangNode(std::string Target)
{
    BaseNode* Targetaddress = findWord(Target);
    if (Targetaddress == nullptr)
    {
        //std::cout << "Target does not exist" << std::endl;
    }
    Targetaddress->getParent()->removeChild(Target);
    Targetaddress->removeParent();
    delete Targetaddress;
}
BaseNode* LanguageTree::findTitle(std::string Target)
{
    return start->getChild(Target);
}