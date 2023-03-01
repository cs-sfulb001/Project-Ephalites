#include "MemoryMenu.h"
#include "Choices.h"
#include "Options.h"
#include "WAAI.h"
#include "UIHelper.h"

void MainMenu()
{
    Options OptionsMenu;
    OptionsMenu.LoadFile();
    MemoryMenu* Mem = nullptr;
    WAAI* Ephalites=nullptr;
    bool running = 1;
    while (running)
    {
        header("Main Menu", 100, '_');
        cout << "What you you like to boot?" << endl;
        switch (choiceList({"Memory Maninpulation","WAAI","Options","Exit"}))
        {
        default:
            cout << "That is not an option" << endl;
            break;
        case 0:
            Mem = new MemoryMenu(OptionsMenu.minTimesUsed(), OptionsMenu.MaxRAM());
            Mem->MemoryMenuStart();
            delete Mem;
            Mem = nullptr;
            break;
        case 1:
            Ephalites=new WAAI;
            Ephalites->startUp();
            delete Ephalites;
            Ephalites = nullptr;
            break;
        case 2:
            OptionsMenu.printOptions();
            OptionsMenu.SaveFile();
            break;
        case 3:
            running = 0;
            break;
        }
    }
    
}

int main(int argc, char const* argv[])
{
    MainMenu();
    return 0;
}
