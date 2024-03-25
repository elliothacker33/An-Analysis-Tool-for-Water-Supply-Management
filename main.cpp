#include "controller/manager.h"
#include "view/menu.h"

int main(){
    Manager* manager = new Manager();
    Menu* menu = new Menu(manager);
    menu->mainMenu();
    return 0;
}

