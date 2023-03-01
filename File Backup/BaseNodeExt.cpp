#include "BaseNodeExt.h"
BaseNodeExt::BaseNodeExt():BaseNode()
{
	CTEqiv = nullptr;
}
BaseNodeExt::BaseNodeExt(std::string word, bool title, int times):BaseNode(word, title, times)
{
	CTEqiv = new CatNode(BaseNode::getWordRef(), BaseNode::getTimesUsedRef());
}
BaseNodeExt::BaseNodeExt(BaseNode& oldNode):BaseNode(oldNode)
{
	CTEqiv = new CatNode(BaseNode::getWordRef(), BaseNode::getTimesUsedRef());
}
BaseNodeExt::~BaseNodeExt()
{
	delete CTEqiv;
	CTEqiv = nullptr;	
}
CatNode* const BaseNodeExt::getCTEqiv()
{
	return &(* CTEqiv);
}