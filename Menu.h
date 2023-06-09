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


//Wyświetla ekran powitalny oraz opcje wyboru menu głównego
void MainMenu(void);

//Czyści terminal i tworzy podkreślenie. Używana do przechodzenia do innych sekcji menu
void refresh(void);

//Wyświetla ekran logowania, oraz menu obsługi konta klienta systemu
void ClientMenu(int& MenuOption);

//Wyświetla ekran logowania, oraz menu obsługi konta administratora systemu
void AdminMenu(int& MenuOption);

//Wyświetla menu edycji parametrów istniejącego konta klienta
void ClientEditMenu(Client& CliObj, Admin& AdmObj);

//Wyświetla menu edycji parametrów istniejącego filmu
void MovieEditMenu(Movie& MovObj);

//Wyświetla menu tworzenia użytkownika systemu
void CreateMenu(int& MenuOption);

#endif // !MENU_H
