#include "MemoryMenu.h"
#include "Choices.h"
#include "UIHelper.h"
MemoryMenu::MemoryMenu():MemoryMenu(0, 2){}
MemoryMenu::MemoryMenu(int minTU, double maxRAM)
{
    Control = new TreeControl(minTU, maxRAM);
}
MemoryMenu::~MemoryMenu()
{
    delete Control;
    Control = nullptr;
}
void MemoryMenu::MemoryMenuStart()
{
    
    std::cout << "Start" << std::endl;
    std::cout << "Building Trees" << std::endl;
    Control->buildTrees();
    std::cout << "Done building Trees" << std::endl;//Something happened when min times used = 3
    bool Run = 1;
    while (Run)
    {
        header("Memory Menu", 80, '_');
        Control->PrintMemoryUsage();
        std::cout << "What would you like to do?" << std::endl;
        switch (choiceList({ "Print Both Trees", "Print Collisions",
            "Add word to Language Tree", "Make Connection in Category Tree","Exit" }))
        {
        case 4:
            Run = 0;
            break;
        case 0:
            Control->PrintTrees();
            break;
        case 1:
            Control->printCollisions();
            break;
        case 2:
            Control->AddToLTree();
            break;
        case 3:
            Control->ConnectInCTree();
            break;
        default:
            std::cout << "That is not an option" << std::endl;
            break;
        }
    }
    Control->SaveTrees();
}