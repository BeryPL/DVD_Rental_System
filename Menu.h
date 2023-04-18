#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <windows.h>
//#include <algorithm>
//#include <vector>
#include <string>
//#include <cstdlib>
#include <iomanip>
#include "Structs.h"
#include "User_Class.h"
#include "Functions.h"


//Wyœwietla ekran powitalny oraz opcje wyboru menu g³ównego
void MainMenu(void);

//Czyœci terminal i tworzy podkreœlenie. U¿ywana do przechodzenia do innych sekcji menu
void refresh(void);

//Wyœwietla ekran logowania, oraz menu obs³ugi konta klienta systemu
void ClientMenu(int& MenuOption);

//Wyœwietla ekran logowania, oraz menu obs³ugi konta administratora systemu
void AdminMenu(int& MenuOption);

//Wyœwietla menu edycji parametrów istniej¹cego konta klienta
void ClientEditMenu(Client& CliObj, Admin& AdmObj);

//Wyœwietla menu edycji parametrów istniej¹cego filmu
void MovieEditMenu(Movie& MovObj);

//Wyœwietla menu tworzenia u¿ytkownika systemu
void CreateMenu(int& MenuOption);

#endif // !MENU_H
