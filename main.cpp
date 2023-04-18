#include <iostream>
#include "Globals.h"
#include "Structs.h"
#include "User_Class.h"
#include "Functions.h"
#include "Menu.h"

int main() {
	GetLocalTime();
	int MenuOption{};
	std::string Menu;

	while (1) {  //Pêtla g³ówna
		MainMenu();
		std::getline(std::cin, Menu);
		try {
			MenuOption = std::stoi(Menu);
		}
		catch (std::invalid_argument&) {
			continue;
		}
		switch (MenuOption) {
			case 1:  //Obs³uga klienta
				ClientMenu(MenuOption);
				MenuOption = 0;
				break;
			case 2:  //Obs³uga administratora
				AdminMenu(MenuOption);
				MenuOption = 0;
				break;
			case 3:  //Tworzenie konta
				CreateMenu(MenuOption);
				MenuOption = 0;
				break;
			default:
				break;
			}
			if (MenuOption == 4) {  //Zamkniêcie programu
				break;
			}
	}
}