#ifndef CATEGORYTREE_H
#define CATEGORYTREE_H

#include "CatNode.h"

class CategoryTree
{
private:
    CatNode* start;
    //Saves from having to delete the start word and timesused
    std::string startWord = "Category";
    int StartTimesUsed = 0;

public:
    /*
    Intailizes all varibles to default values.
    */
    CategoryTree();
    /*
    Depends heavily on CatNode destructor at the
    moment. Does not take control of the trees destruction
    process.
    */
    ~CategoryTree();
    void SaveTree();
    void SaveTree(CatNode*, std::vector<std::string>, std::vector<CatNode*>, std::ofstream&);
    void ConnectNodes(CatNode*, CatNode*, std::vector<std::string>*);
    bool CheckCollisionCreation(CatNode*, std::string);
    /*
    When a new collision point is detected, collision
    parents must be updated. Everything after the new
    collision point is then updated to contain the first
    parent of the collision point as a collision parent.
    */
    void CollisionBackFill(CatNode*, std::vector<std::string>);
    void CollisionFill(CatNode*, int, CatNode*, std::vector<std::string>);
    CatNode* getStart();
    /*
    Starts the print function for Category Tree
    sending default agruments to the recursive function
    */
    void PrintLoadedTree();
    /*
    Recursive print function that takes inputs of the
    node to print and tries to print each child if they
    are part of the current path.
    */
    void PrintLoadedTree(CatNode*, std::vector<std::string>, int);
    void PrintUnloadedTree();
    /*
    Prints the input node
    */
    void PrintNode(CatNode*);
};
#endif