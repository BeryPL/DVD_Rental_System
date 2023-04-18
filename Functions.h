#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "Structs.h"
#include "User_Class.h"

//Funkcja pobiera aktualny czas z systemu do struktury globalnej LocalTimeNow
void GetLocalTime(void);

//Funkcja por�wnuje ze sob� tytu� w strukturze BorrowedStruct z tytu�em podanym
bool CompareTitle(const BorrowedStruct& In, const std::string& Title);

//Wy�wietla loginy klient�w istniej�cych w systemie z pliku ClientsList.txt
void PrintClients(void);

//Funkcja zwraca tytu� wypo�yczony przez klienta, je�li klient ma go wypo�yczonego
bool ReturnMovie(Client& CliObj, std::string Movie);

//Funkcja wypo�ycza film. Tworzy dat� oddania na bazie aktualnej i konta klienta i dodaje rekord do BorrowedTitles.txt
int BorrowMovie(Client& CliObj, std::string& Movie);

//Funkcja tworz�ca nowe konto klienta
void CreateClientAccount(void);

//Funkcja zapisuj�ca nowo utworzone konto do plik�w
void SaveNewAccount(const Client& obj);

//Funkcja do zapisywania nowego has�a klienta po zmianach
void SaveNewClientPass(const Client& obj);

//Funkcja do zapisu konta klienta po zmianach
void SaveAccount(const Client& obj, const std::string& key);

//Funkcja poszukuj�ca konto klienta i sprawdzaj�ca podane has�o do konta
int FindClientAccount(const std::string key, const std::string pass);

//Funkcja sprawdzaj�ca istnienie konta klienta w bazie
bool FindClientAccount(const std::string key);

//Funkcja �aduj�ca dane klienta do obiektu klasy klient na podstawie loginu
void LoadAccount(const std::string key, std::shared_ptr<Client>& InObj);

//Funkcja zapisuj�ca nowo utworzone konto admina do plik�w
void SaveNewAccount(const Admin& obj);

//Funkcja tworz�ca nowe konto administratora
void CreateAdminAccount(void);

//Funkcja do zapisu konta admina po zmianach
void SaveAccount(Admin& obj, const std::string& key);

//Funkcja poszukuj�ca konto admina i sprawdzaj�ca podane has�o do konta
int FindAdminAccount(const std::string key, const std::string pass);

//Funkcja �aduj�ca dane admina do obiektu klasy Admin na podstawie loginu
void LoadAccount(const std::string key, std::shared_ptr<Admin>& InObj);

//Funkcja �aduj�ca dane filmu do obiektu klasy Movie na bazie tytu�u
void LoadMovie(const std::string key, std::shared_ptr<Movie>& InObj);

//Funkcja zapisuj�ca nowy film do pliku
void SaveNewMovie(const Movie& obj);

//Funkcja tworz�ca nowy obiekt Movie
void CreateMovie(void);

//Funkcja do zampisu filmu po edycji
void SaveMovie(Movie& obj, const std::string& key);

//Funkcja sprawdzaj�ca istnienie filmu o podanym tytule w bazie
bool FindMovie(std::string Title);

//Funkcja wy�wietlaj�ca list� dost�pnych film�w
void PrintMoviesList(void);

//Funkcja por�wnuj�ca dwie daty, zwraca true gdy Date2 > Date1
bool CompDate(const DateStruct& Date1, const DateStruct& Date2);

//Funkcja wy�wietlaj�ca wypo�yczone filmy i wypo�yczaj�cego
void PrintRented(void);

//Funkcja wy�wietlaj�ca filmy nie oddane w terminie
void PrintOverdue(const DateStruct& CurrentDate);


#endif // !FUNCTIONS_H

