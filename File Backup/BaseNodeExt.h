#ifndef BASENODEEXT_H
#define BASENODEEXT_H

#include "BaseNode.h"
#include "CatNode.h"
class BaseNodeExt : public BaseNode
{
public:
	BaseNodeExt();
	BaseNodeExt(std::string, bool, int);
	BaseNodeExt(BaseNode&);
	~BaseNodeExt();
	CatNode* const getCTEqiv();

private:
	CatNode* CTEqiv;
	//std::string Word;
	//bool Title;
	//BaseNode* Parent;
	//int timesUsed;
	//std::vector<BaseNode*> Children;
};


#endif // !BASENODEEXT_H