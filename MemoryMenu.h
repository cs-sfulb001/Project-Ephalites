#pragma once
#include"TreeControl.h"
class MemoryMenu
{
private:
	TreeControl* Control;
public:
	MemoryMenu();
	MemoryMenu(int, double);
	~MemoryMenu();
	void MemoryMenuStart();
};

