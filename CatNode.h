#pragma once
#include <iomanip>
#include <vector>
#include <iostream>
#include <unordered_map>

class CatNode
{
public:
    CatNode();
    CatNode(std::string, int, int);
    CatNode(std::string&, int&);
    ~CatNode()=default;//Removing the custom destructor resulted in a smaller memory leak
    //sets
    void setCollisionStatus(bool);
    //gets
    std::string const getWord();
    int const getTimesUsed();
    int const getNumChildren();
    int const getNumParents();
    CatNode* const getChild(std::string);
    CatNode* const getParent(std::string);
    std::unordered_map<int, CatNode*>::iterator getChildernBegin();
    std::unordered_map<int, CatNode*>::iterator getChildernEnd();
    std::unordered_map<int, CatNode*>::iterator getParentsBegin();
    std::unordered_map<int, CatNode*>::iterator getParentsEnd();
    bool const getIsCollision();
    bool const isParent(std::string);
    bool const isParent(CatNode&);
    bool const getAnyCollisionParents();
    std::vector<std::string> getCollisionList(int);
    bool const getAnyParents();
    //adds
    /*
    Adds the last object of input vector to
    CatCollisionParents if it finds a corisponding path
    or adds a new path
    */
    void AddCollisionParent(std::vector<std::string>);
    /*
    Adds the new collision parent to the corispoding list
    by comparing the input list without the new term
    */
    void AddCollisionParent(std::vector<std::string>, int);
    void addParent(CatNode&);
    void addChild(CatNode&);
    //Removes
    void removeParent(std::string);
    void removeChild(std::string);
    //Data Manipulation
    bool const checkCollisionList(std::vector<std::string>);

    /*
    Prints information of node based on LTEquiv to desired ostream
    */
    void print(std::ostream&);
    void PrintCollisions();
    std::string* word;

private:
    int* TimesUsed;
    bool title;
    std::unordered_map <int, CatNode*> Children;
    std::unordered_map <int, CatNode*> Parents;
    std::vector<std::vector <std::string>> CollisionParent;
    bool isCollision;
};

template<>
struct std::hash<CatNode>
{
    std::size_t operator()(const CatNode& node) const noexcept
    {
        return std::hash<std::string>{}(*node.word);
    }
};