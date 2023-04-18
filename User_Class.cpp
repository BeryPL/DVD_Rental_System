#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "User_Class.h"
#include "Structs.h"
#include "Globals.h"
#include "Functions.h"


//Konstruktor klasy User
User::User(std::string Login, std::string Password, std::string UsName, std::string UsSurname, DateStruct Date, int PhoneNb, std::string eMail): fLogin{ Login }, fPassword{ Password }, fUserName{ UsName }, fUserSurname{ UsSurname }, fCreationDate{ Date }, fPhoneNb{ PhoneNb }, feMail{ eMail } {}

//Gettery dla klasy User
std::string User::get_Login(void) const { return fLogin; }
std::string User::get_Password(void) const { return fPassword; }
DateStruct User::get_CreationDate(void) const { return fCreationDate; }
int User::get_PhoneNb(void) const { return fPhoneNb; }
std::string User::get_eMail(void) const { return feMail; }
std::string User::get_UserName(void) const { return fUserName; }
std::string User::get_UserSurname(void) const { return fUserSurname; }

//Settery dla klasy User
void User::set_eMail(std::string& eMail) { feMail = eMail; }
void User::set_PhoneNb(int& PhoneNb) { fPhoneNb = PhoneNb; }
void User::set_Password(std::string& NewPass) { fPassword = NewPass; }

//Konstruktor klasy Client
Client::Client(std::string Login, std::string Password, std::string ClName, std::string ClSurname, int PhoneNb, std::string eMail,
	int MaxMovieNb, int BorrowedNb, std::string CliRank, DateStruct Date)
	: User(Login, Password, ClName, ClSurname, Date, PhoneNb, eMail), fMaxMovieNb{ MaxMovieNb }, fBorrowedNb{ BorrowedNb }, fClientRank{ CliRank } {}


//Gettery klasy Client
std::string Client::get_AccountType(void) const { return fAccountType; }
int Client::get_BorrowedNb(void) const { return fBorrowedNb; }
int Client::get_MaxMovieNb(void) const { return fMaxMovieNb; }
std::string Client::get_ClientRank(void) const { return fClientRank; }
std::vector<BorrowedStruct> Client::get_BorrowedTitles(void) const { return fBorrowedTitles; }

//Settery klasy Client
void Client::add_BorrowedTitle(const BorrowedStruct& Movie) { //Dodaje wypo¿yczony tytu³ do wektora klasy
	fBorrowedTitles.push_back(Movie);
	fBorrowedNb++;
} 
bool Client::remove_BorrowedTitle(const std::string& Title) {  //Usuwa wypo¿yczony tytu³ z wektora klasy
	auto pos = std::find_if(fBorrowedTitles.begin(), fBorrowedTitles.end(), [Title](const BorrowedStruct& Element) {
		return CompareTitle(Element, Title);
		});
	if (pos != fBorrowedTitles.end()) {
		fBorrowedTitles.erase(pos);
		fBorrowedNb--;
		return true;
	}
	return false;
}
void Client::SaveBorrowedTitles(void) {  //Zapisuje wektor wypo¿yczonych tytu³ów do pliku
	std::string line;
	std::ifstream InFile("BorrowedTitles.txt");
	std::ofstream OutFile("temp.txt", std::ios::trunc);
	while (std::getline(InFile, line)) {
		if (line == this->get_Login()) {
			for (int i = 0; i < 7; i++) {
				std::getline(InFile, line);
			}
		}
		else {
			OutFile << line << '\n';
		}
	}
	InFile.close();
	for (auto Movie : fBorrowedTitles) {
		OutFile << this->get_Login() << '\n';
		OutFile << Movie.Title << '\n';
		OutFile << Movie.BDate.Day << '\n';
		OutFile << Movie.BDate.Month << '\n';
		OutFile << Movie.BDate.Year << '\n';
		OutFile << Movie.RDate.Day << '\n';
		OutFile << Movie.RDate.Month << '\n';
		OutFile << Movie.RDate.Year << '\n';
	}
	OutFile.close();
	if (std::remove("BorrowedTitles.txt") != 0) {
		std::cerr << "Error";
	}

	if (std::rename("temp.txt", "BorrowedTitles.txt") != 0) {
		std::cerr << "Error";
	}
}
void Client::LoadBorrowedTitles(void) {  //Pobiera wektor wypo¿yczonych filmów z pliku
	BorrowedStruct Movie{};
	std::string line;
	int LineCount{ 0 };
	std::ifstream InFile("BorrowedTitles.txt");
	if (!InFile.is_open()) {
		return;
	}
	while (std::getline(InFile, line)) {
		if (LineCount % 8 == 0) {
			if (line == this->get_Login()) {
				std::getline(InFile, line);
				Movie.Title = line;
				std::getline(InFile, line);
				try {
					Movie.BDate.Day = std::stoi(line);
				} catch (std::invalid_argument&) {
					InFile.close();
					return;
				}
				std::getline(InFile, line);
				try {
					Movie.BDate.Month = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFile.close();
					return;
				}
				std::getline(InFile, line);
				try {
					Movie.BDate.Year = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFile.close();
					return;
				}
				std::getline(InFile, line);
				try {
					Movie.RDate.Day = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFile.close();
					return;
				}
				std::getline(InFile, line);
				try {
					Movie.RDate.Month = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFile.close();
					return;
				}
				std::getline(InFile, line);
				try {
					Movie.RDate.Year = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFile.close();
					return;
				}
				fBorrowedTitles.push_back(Movie);
				LineCount += 7;
			}
		}
		LineCount++;
	}
	InFile.close();
}

//Przeci¹¿enie operatora wyjœcia do wyœwietlania klasy Client
std::ostream& operator<< (std::ostream& out, Client& ClientObj) {
	out << "Login: " << ClientObj.get_Login() << std::endl;
	out << "Name: " << ClientObj.get_UserName() << std::endl;
	out << "Surname: " << ClientObj.get_UserSurname() << std::endl;
	out << "Phone: " << ClientObj.get_PhoneNb() << std::endl;
	out << "email: " << ClientObj.get_eMail() << std::endl;
	out << "Account type: " << ClientObj.get_AccountType() << std::endl;
	out << "Created: " << ClientObj.get_CreationDate();
	out << "Maximum amount of movies to borrow: " << ClientObj.get_MaxMovieNb() << std::endl;
	out << "Amount of currently borrowed movies: " << ClientObj.get_BorrowedNb() << std::endl;
	out << "Client rank: " << ClientObj.get_ClientRank() << std::endl;
	out << "Currently borrowed movies:" << std::endl;
	for (auto BorrowedMovie : ClientObj.get_BorrowedTitles()) {
		out << "- " << BorrowedMovie.Title << std::endl;
	}
	return out;
}

//Konstruktor klasy Admin
Admin::Admin(std::string Login, std::string Password, std::string AdName, std::string AdSurname, int PhoneNb, std::string eMail, std::string EmpTitle, DateStruct Date)
	: User(Login, Password, AdName, AdSurname, Date, PhoneNb, eMail), fEmployeeTitle{ EmpTitle } {}

//Gettery dla kklasy Admin
std::string Admin::get_EmployeeTitle(void) const { return fEmployeeTitle; }
std::string Admin::get_AccountType(void) const { return fAccountType; }

//Settery klasy Admin do obs³ugi klasy Client
void Admin::set_ClientMaxMovieNb(Client& obj, int nb) { obj.fMaxMovieNb = nb; }
void Admin::set_IncrementUserBorrowedNb(Client& obj) { obj.fBorrowedNb++; }
void Admin::set_DecrementUserBorrowedNb(Client& obj) { obj.fBorrowedNb--; }
void Admin::set_AddRecordToBorrowedTitles(Client& obj, BorrowedStruct& NewBorrow) { obj.fBorrowedTitles.push_back(NewBorrow); }
void Admin::set_ClearRecordFromBorrowedTitles(Client& obj, std::string& Title) {
	std::vector<BorrowedStruct>::iterator Here;
	for (auto pos = obj.fBorrowedTitles.begin(); pos != obj.fBorrowedTitles.end(); ++pos) {
		if (Title == pos->Title) {
			Here = pos;
		}
	}
	obj.fBorrowedTitles.erase(Here);
}
void Admin::set_ClientPassword(Client& obj, std::string pass) { obj.set_Password(pass); }
void Admin::set_ClientName(Client& obj, std::string name) { obj.fUserName = name; }
void Admin::set_ClientSurname(Client& obj, std::string surname) { obj.fUserSurname = surname; }
void Admin::set_ClientPhone(Client& obj, int phone) { obj.set_PhoneNb(phone); }
void Admin::set_ClientrEmail(Client& obj, std::string email) { obj.set_eMail(email); }
void Admin::set_ClientRank(Client& obj, int i) {
	if (i == 1) {
		obj.fClientRank = "Standard";
		obj.fMaxMovieNb = 10;
		return;
	}
	else if (i == 2) {
		obj.fClientRank = "Premium";
		obj.fMaxMovieNb = 15;
		return;
	}
	return;
}

//Gettery dla klasy Admin do obs³ugi klasy Client
int Admin::get_ClientMaxMovieNb(const Client& obj) const { return obj.fBorrowedNb; }
std::vector<BorrowedStruct> Admin::get_ClientBorrowedTitles(const Client& obj) const { return obj.fBorrowedTitles; }

//Przeci¹¿enie operatora wyjœcia do wyœwietlania klasy Admin
std::ostream& operator<< (std::ostream& out, const Admin& AdminObj) {
	out << "Login: " << AdminObj.get_Login() << std::endl;
	out << "Name: " << AdminObj.get_UserName() << std::endl;
	out << "Surname: " << AdminObj.get_UserSurname() << std::endl;
	out << "Work position: " << AdminObj.get_EmployeeTitle() << std::endl;
	out << "Phone: " << AdminObj.get_PhoneNb() << std::endl;
	out << "email: " << AdminObj.get_eMail() << std::endl;
	out << "Account type: " << AdminObj.get_AccountType() << std::endl;
	out << "Created: " << AdminObj.get_CreationDate();
	return out;
}

//Konstruktor klasy Movie
Movie::Movie(DateStruct RelDate, TimeStruct MovLength, std::vector<std::string> Actors, std::string Info, std::string Title, std::string Genre,
	std::string Director, int InStore, int Available)
	: fMovieTitle{ Title }, fGenre{ Genre }, fDirector{ Director }, fReleaseDate{ RelDate }, fMovieLength{ MovLength }, fMainActors{ Actors }, fMovieInfo{ Info },
	fAmountInStore{ InStore }, fAmountAvailable{ Available } {}

//Gettery klasy Movie
std::string Movie::get_MovieTitle(void) const { return fMovieTitle; }
std::string Movie::get_Genre(void) const { return fGenre; }
std::string Movie::get_Director(void) const { return fDirector; }
DateStruct Movie::get_ReleaseDate(void) const { return fReleaseDate; }
TimeStruct Movie::get_MovieLength(void) const { return fMovieLength; }
std::vector<std::string> Movie::get_MainActors(void) const { return fMainActors; }
std::string Movie::get_MovieInfo(void) const { return fMovieInfo; }
int Movie::get_AmountInStore(void) const { return fAmountInStore; }
int Movie::get_AmountAvailable(void) const { return fAmountAvailable; }

//Settery klasy Movie
void Movie::set_Genre(const std::string& Genre) { fGenre = Genre; }
void Movie::set_Director(const std::string& Director) { fDirector = Director; }
void Movie::set_ReleaseDate(const DateStruct& RDate) { fReleaseDate = RDate; }
void Movie::set_MovieLength(const TimeStruct& Time) { fMovieLength = Time; }
void Movie::set_Actors(const std::vector<std::string>& Actors) { fMainActors = Actors; }
void Movie::set_MovieInfo(const std::string& Info) { fMovieInfo = Info; }
void Movie::set_InStore(const int& InStore) { fAmountInStore = InStore; }
void Movie::set_Available(const int& Available) { fAmountAvailable = Available; }
void Movie::add_actor(const std::string& Actor) { fMainActors.push_back(Actor); }

//Przeci¹¿enie operatora wyjœcia do wyœwietlania klasy Movie
std::ostream& operator<< (std::ostream& out, Movie& MovObj) {
	out << "Title: " << MovObj.get_MovieTitle() << std::endl;
	out << "Genre: " << MovObj.get_Genre() << std::endl;
	out << "Director: " << MovObj.get_Director() << std::endl;
	out << "Release date: " << MovObj.get_ReleaseDate();
	out << "Movie length: " << MovObj.get_MovieLength();
	out << "Cast: ";
	for (auto Actor : MovObj.get_MainActors()) {
		out << Actor << ", ";
	}
	out << std::endl;
	out << "Info: " << MovObj.get_MovieInfo() << std::endl;
	out << "Available: " << MovObj.get_AmountAvailable() << std::endl;
	return out;
}