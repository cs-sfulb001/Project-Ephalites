#ifndef TREECONTROL_H
#define TREECONTROL_H

#include "LanguageTree.h"
#include "CategoryTree.h"
#include "MemoryToolWOS.h"

class TreeControl
{
private:
    LanguageTree LTree;
    CategoryTree CTree;
    MemoryToolWOS RAM;
public:
    /*
    Until there is a pointer used the constructor does
    not contain anything
    */

    TreeControl();
    TreeControl(int, double);

    //Nothing requires a destructor to be implemented

    ~TreeControl()=default;

    /*
    Calls all functions for each tree in class to be
    built from a file
    */
    void buildTrees();
    /*
    Because CTree uses LTree as a master list, CTree
    build functions must be in a class that has both
    CTree and LTree
    */
    void printCollisions();
    void printCollisions(BaseNode*);
    void LoadCTreeFile();
    void SaveTrees();
    /*
    Calls each Tree's print function
    */
    void PrintTrees();
    void AddToLTree();
    void AddToLTree(std::string);
    BaseNodeExt* morphBaseNode(BaseNode*);
    void ConnectInCTree();
    void ConnectInCTree(BaseNode*);
    void ConnectInCTree(CatNode*, CatNode*);
    LanguageTree* GetLT();
};
#endif