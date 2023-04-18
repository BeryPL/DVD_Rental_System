#ifndef USER_CLASS_H
#define USER_CLASS_H

#include <string>
#include <vector>
#include "Structs.h"
#include "Globals.h"

//Klasa rodzicielska dla klas kont u�ytkownik�w, posiada najwa�niejsze dane kont u�ytkownik�w jak: login, imie, nazwisko itp.
class User {
	friend class Admin;					 //Zaprzyja�nienie z klas� admin w celu umo�liwiwenia edycji parametr�w konta klienta z poziomu admin
private:
	std::string fLogin;					 //login
	std::string fPassword;				 //has�o do konta
	std::string fUserName;				 //Imie u�ytkownika
	std::string fUserSurname;			 //Nazwisko u�ytkownika
	DateStruct fCreationDate;			 //Data utworzenia konta
	int fPhoneNb{};						 //Nr telefonu u�ytkownika
	std::string feMail;					 //Adres email u�ytkownika
public:
	//Konstruktor klasy User
	User(std::string Login = "Default", std::string Password = "Default", std::string UsName = "none", std::string UsSurname = "none", DateStruct Date = { 1900 + LocalTimeNow.tm_year, 1 + LocalTimeNow.tm_mon, LocalTimeNow.tm_mday }, int PhoneNb = 0, std::string eMail = "none");
	
	//Gettery dla klasy User
	std::string get_Login(void) const;
	std::string get_Password(void) const;
	DateStruct get_CreationDate(void) const;
	int get_PhoneNb(void) const;
	std::string get_eMail(void) const;
	std::string get_UserName(void) const;
	std::string get_UserSurname(void) const;
	
	//Settery dla klasy User
	void set_eMail(std::string& eMail);
	void set_PhoneNb(int& PhoneNb);
	void set_Password(std::string& NewPass);
};

//Klasa dziedzicz�ca z User do obs�ugi konta typu klient
class Client : public User {
	friend class Admin;								 //Zaprzyja�nienie z klas� admin w celu umo�liwiwenia edycji parametr�w konta klienta z poziomu admina
private:
	const std::string fAccountType = "Client";		 //Sta�a okre�laj�ca typ konta
	int fMaxMovieNb{};								 //Okre�la maksymaln� liczb� mo�liwych do wypo�yczenia film�w
	int fBorrowedNb{};								 //Okre�la aktualn� liczb� wypo�yczonych film�w
	std::vector<BorrowedStruct> fBorrowedTitles;	 //Przechowuje struktury BorrowedStruct z danymi wypo�yczonych film�w
	std::string fClientRank;						 //Okre�la rang� klienta, wykorzystywane do okre�lenia terminu zwrotu filmu
public:
	//Konstruktor klienta
	Client(std::string Login = "Client", std::string Password = "Client", std::string ClName = "none", std::string ClSurname = "none", int PhoneNb = 0, std::string eMail = "none", int MaxMovieNb = 10, int BorrowedNb = 0, std::string CliRank = "Standard",
		DateStruct Date = { 1900 + LocalTimeNow.tm_year, 1 + LocalTimeNow.tm_mon, LocalTimeNow.tm_mday });

	//Gettery dla klasy Client
	std::string get_AccountType(void) const;
	int get_BorrowedNb(void) const;
	int get_MaxMovieNb(void) const;
	std::string get_ClientRank(void) const;
	std::vector<BorrowedStruct> get_BorrowedTitles(void) const;

	//Settery dla klasy klienta
	void add_BorrowedTitle(const BorrowedStruct& Movie);
	bool remove_BorrowedTitle(const std::string& Title);
	void SaveBorrowedTitles(void);
	void LoadBorrowedTitles(void);
};

//Przeci��ony operator wyj�cia do wy�wietlania klasy Client
std::ostream& operator<< (std::ostream& out, Client& ClientObj);

//Klasa dziedzicz�ca z User do obs�ugi konta typu admin
class Admin : public User {
private:
	const std::string fAccountType = "Admin";	//Sta�a okre�laj�ca typ konta
	std::string fEmployeeTitle;					//Sta�a okre�laj�ca stanowisko pracy w�a�ciciela konta
public:
	//Konstruktor klasy admin
	Admin(std::string Login = "Admin", std::string Password = "Admin", std::string AdName = "none", std::string AdSurname = "none", int PhoneNb = 0, std::string eMail = "none", std::string EmpTitle = "Default",
		DateStruct Date = { 1900 + LocalTimeNow.tm_year, 1 + LocalTimeNow.tm_mon, LocalTimeNow.tm_mday });

	//Gettery klasy admin
	std::string get_EmployeeTitle(void) const;
	std::string get_AccountType(void) const;

	//Settery klasy admin do obs�ugi klasy Client (konta u�ytkownika)
	void set_ClientMaxMovieNb(Client& obj, int nb);
	void set_IncrementUserBorrowedNb(Client& obj);
	void set_DecrementUserBorrowedNb(Client& obj);
	void set_AddRecordToBorrowedTitles(Client& obj, BorrowedStruct& NewBorrow);
	void set_ClearRecordFromBorrowedTitles(Client& obj, std::string& Title);
	void set_ClientPassword(Client& obj, std::string pass);
	void set_ClientName(Client& obj, std::string name);
	void set_ClientSurname(Client& obj, std::string surname);
	void set_ClientPhone(Client& obj, int phone);
	void set_ClientrEmail(Client& obj, std::string email);
	void set_ClientRank(Client& obj, int i);

	//Gettery klasy admin do obs�ugi klasy Client (konta u�ytkownika)
	int get_ClientMaxMovieNb(const Client& obj) const;
	std::vector<BorrowedStruct> get_ClientBorrowedTitles(const Client& obj) const;

};
//Przeci��ony operator wyj�cia do wy�wietlania klasy Admin
std::ostream& operator<< (std::ostream& out, const Admin& AdminObj);

//Klasa Movie do przechowywania danych o filmie dost�pnym w wypo�yczalni
class Movie {
private:
	std::string fMovieTitle;					 //Tytu�
	std::string fGenre;							 //Gatunek
	std::string fDirector;						 //Re�yser
	DateStruct fReleaseDate;					 //Data publikacji
	TimeStruct fMovieLength;					 //Czas trwania filmu
	std::vector<std::string> fMainActors;		 //Przechowuje imiona i nazwiska g��wnych aktor�w graj�cych w filmie
	std::string fMovieInfo;						 //Opis fabu�y filmu
	int fAmountInStore;							 //Okre�la ilo�� sztuk na magazynie
	int fAmountAvailable;						 //Okre�la ilo�� sztuk dost�pnych do wypo�yczenia
public:
	//Konstruktor klasy Movie
	Movie(DateStruct RelDate, TimeStruct MovLength, std::vector<std::string> Actors, std::string Info, std::string Title = "none", std::string Genre = "none",
		std::string Director = "none", int InStore = 0, int Available = 0);

	//Gettery klasy Movie
	std::string get_MovieTitle(void) const;
	std::string get_Genre(void) const;
	std::string get_Director(void) const;
	DateStruct get_ReleaseDate(void) const;
	TimeStruct get_MovieLength(void) const;
	std::vector<std::string> get_MainActors(void) const;
	std::string get_MovieInfo(void) const;
	int get_AmountInStore(void) const;
	int get_AmountAvailable(void) const;

	//Settery klasy Movie
	void set_Genre(const std::string& Genre);
	void set_Director(const std::string& Director);
	void set_ReleaseDate(const DateStruct& RDate);
	void set_MovieLength(const TimeStruct& Time);
	void set_Actors(const std::vector<std::string>& Actors);
	void set_MovieInfo(const std::string& Info);
	void set_InStore(const int& InStore);
	void set_Available(const int& Available);
	void add_actor(const std::string& Actor);

};

//Przeci��ony operator wyj�cia do wy�wietlania klasy Admin
std::ostream& operator<< (std::ostream& out, Movie& MovObj);

#endif // !User_Class
