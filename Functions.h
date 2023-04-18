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

//Funkcja porównuje ze sob¹ tytu³ w strukturze BorrowedStruct z tytu³em podanym
bool CompareTitle(const BorrowedStruct& In, const std::string& Title);

//Wyœwietla loginy klientów istniej¹cych w systemie z pliku ClientsList.txt
void PrintClients(void);

//Funkcja zwraca tytu³ wypo¿yczony przez klienta, jeœli klient ma go wypo¿yczonego
bool ReturnMovie(Client& CliObj, std::string Movie);

//Funkcja wypo¿ycza film. Tworzy datê oddania na bazie aktualnej i konta klienta i dodaje rekord do BorrowedTitles.txt
int BorrowMovie(Client& CliObj, std::string& Movie);

//Funkcja tworz¹ca nowe konto klienta
void CreateClientAccount(void);

//Funkcja zapisuj¹ca nowo utworzone konto do plików
void SaveNewAccount(const Client& obj);

//Funkcja do zapisywania nowego has³a klienta po zmianach
void SaveNewClientPass(const Client& obj);

//Funkcja do zapisu konta klienta po zmianach
void SaveAccount(const Client& obj, const std::string& key);

//Funkcja poszukuj¹ca konto klienta i sprawdzaj¹ca podane has³o do konta
int FindClientAccount(const std::string key, const std::string pass);

//Funkcja sprawdzaj¹ca istnienie konta klienta w bazie
bool FindClientAccount(const std::string key);

//Funkcja ³aduj¹ca dane klienta do obiektu klasy klient na podstawie loginu
void LoadAccount(const std::string key, std::shared_ptr<Client>& InObj);

//Funkcja zapisuj¹ca nowo utworzone konto admina do plików
void SaveNewAccount(const Admin& obj);

//Funkcja tworz¹ca nowe konto administratora
void CreateAdminAccount(void);

//Funkcja do zapisu konta admina po zmianach
void SaveAccount(Admin& obj, const std::string& key);

//Funkcja poszukuj¹ca konto admina i sprawdzaj¹ca podane has³o do konta
int FindAdminAccount(const std::string key, const std::string pass);

//Funkcja ³aduj¹ca dane admina do obiektu klasy Admin na podstawie loginu
void LoadAccount(const std::string key, std::shared_ptr<Admin>& InObj);

//Funkcja ³aduj¹ca dane filmu do obiektu klasy Movie na bazie tytu³u
void LoadMovie(const std::string key, std::shared_ptr<Movie>& InObj);

//Funkcja zapisuj¹ca nowy film do pliku
void SaveNewMovie(const Movie& obj);

//Funkcja tworz¹ca nowy obiekt Movie
void CreateMovie(void);

//Funkcja do zampisu filmu po edycji
void SaveMovie(Movie& obj, const std::string& key);

//Funkcja sprawdzaj¹ca istnienie filmu o podanym tytule w bazie
bool FindMovie(std::string Title);

//Funkcja wyœwietlaj¹ca listê dostêpnych filmów
void PrintMoviesList(void);

//Funkcja porównuj¹ca dwie daty, zwraca true gdy Date2 > Date1
bool CompDate(const DateStruct& Date1, const DateStruct& Date2);

//Funkcja wyœwietlaj¹ca wypo¿yczone filmy i wypo¿yczaj¹cego
void PrintRented(void);

//Funkcja wyœwietlaj¹ca filmy nie oddane w terminie
void PrintOverdue(const DateStruct& CurrentDate);


#endif // !FUNCTIONS_H

