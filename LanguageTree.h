#ifndef LANGUAGETREE_H
#define LANGUAGETREE_H

#include <fstream>
#include "BaseNode.h"
#include "CatNode.h"
#include "BaseNodeExt.h"

/*
    This class is designed to create a tree that will be
    call Language. This tree is to contain words stored
    relitive to grammar.
*/

class LanguageTree
{
private:
    BaseNode* start;
    BaseNode* noun;
    BaseNode* pronoun;
    BaseNode* propernoun;
    BaseNode* verb;
public:
    /*
    Default Constructor creates the starting node
    "Language" which is a title and sets rest of the
    pointers to nullptr
    */
    LanguageTree();
    /*
    Destructor depends heavily on the BaseNode
    destrcutor. This is not a problem curently
    as BaseNode and LanguageTree are not meant to
    be used independent of one another
    */
    ~LanguageTree();
    /*
    Starts print recursive print with input of the
    start node
    */
    void PrintLoadedTree();
    void PrintUnloadedTree();
    /*
    Recursive function that prints the node fed in
    and calls itself with the input of each of said
    nodes children
    */
    void PrintLoadedTree(BaseNode*, int);
    /*
    Loads Tree from a set file
    */
    void LoadTree();
    /*
    */
    void LoadWord(std::string);
    /*
    Outputs to set file
    */
    void SaveTree();
    /*
        finding a node if you only know the word you want.
    */
    BaseNode* getStart();
    BaseNode* findWord(std::string);
    /*
        finding a node if you know the parent of the word
        you want.
    */
    BaseNode* findWord(std::string, std::string);
    bool inFile(std::string);
    int TimesUsedFromFile(std::string);
    /*
    Remove Node from Langage Tree. Needs to be
    combined with Category Tree functions to insure
    Category Tree does not become corupted
    */
    void RemoveLangNode(std::string);
    /*
    Adds a new node to the Language tree with
    specified parent. If parent does not exist it
    creates the parent title;
    */
    void NewNode(std::string, std::string, int);
    /*
    Creates title nodes of specified name. If
    the node has the name of one of the pointers, it
    sets the pointers to the new title node.
    */
    void NewTitleNode(std::string);
    BaseNode* findTitle(std::string);
private:
    /*
    Recursive function that checks if the desired
    word is a desendent of the input node
    */
};


#endif // !LANGUAGETREE_H