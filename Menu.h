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


//Wy�wietla ekran powitalny oraz opcje wyboru menu g��wnego
void MainMenu(void);

//Czy�ci terminal i tworzy podkre�lenie. U�ywana do przechodzenia do innych sekcji menu
void refresh(void);

//Wy�wietla ekran logowania, oraz menu obs�ugi konta klienta systemu
void ClientMenu(int& MenuOption);

//Wy�wietla ekran logowania, oraz menu obs�ugi konta administratora systemu
void AdminMenu(int& MenuOption);

//Wy�wietla menu edycji parametr�w istniej�cego konta klienta
void ClientEditMenu(Client& CliObj, Admin& AdmObj);

//Wy�wietla menu edycji parametr�w istniej�cego filmu
void MovieEditMenu(Movie& MovObj);

//Wy�wietla menu tworzenia u�ytkownika systemu
void CreateMenu(int& MenuOption);

#endif // !MENU_H
