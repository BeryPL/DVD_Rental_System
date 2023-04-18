
#include "Functions.h"
#include "Globals.h"
//#include "User_Class.h"

//Funkcja pobiera aktualny czas z systemu do struktury globalnej LocalTimeNow
void GetLocalTime(void) {
	std::time_t now = std::time(0);
	localtime_s(&LocalTimeNow, &now);
}

//Funkcja porównuje ze sob¹ tytu³ w strukturze BorrowedStruct z tytu³em podanym
bool CompareTitle(const BorrowedStruct& In, const std::string& Title) {
	return In.Title == Title;
}

//Wyœwietla loginy klientów istniej¹cych w systemie z pliku ClientsList.txt
void PrintClients(void) {
	std::ifstream InFile("ClientsList.txt");
	std::string line;
	int LineCount{ 0 };

	while (std::getline(InFile, line)) {
		if (LineCount % 2 == 0) {
			std::cout << line << std::endl;
		}
		LineCount++;
	}
	InFile.close();
}

//Funkcja zwraca tytu³ wypo¿yczony przez klienta, jeœli klient ma go wypo¿yczonego
bool ReturnMovie(Client& CliObj, std::string Movie) {
	if (!CliObj.remove_BorrowedTitle(Movie)) {
		return false;
	}
	std::ifstream InFile("MoviesList.txt");
	std::ofstream OutFile("temp.txt", std::ios::trunc);
	if (!InFile.is_open()) {
		return false;
	}
	if (!OutFile.is_open()) {
		return false;
	}

	int Available{ 0 };
	int LineCount{ 0 };
	std::string line;

	if (InFile.is_open()) {
		while (std::getline(InFile, line)) {
			if (LineCount % 2 == 0) {
				if (line == Movie) {
					OutFile << line << '\n';
					std::getline(InFile, line);
					try {
						Available = std::stoi(line);
					}
					catch (std::invalid_argument&) {
						InFile.close();
						OutFile.close();
						return false;
					}
					OutFile << (Available + 1) << '\n';
					LineCount++;
				}
				else {
					OutFile << line << '\n';
				}
			}
			else {
				OutFile << line << '\n';
			}
			LineCount++;
		}
	}
	InFile.close();
	OutFile.close();
	if (std::remove("MoviesList.txt") != 0) {
		std::cerr << "Error";
	}

	if (std::rename("temp.txt", "MoviesList.txt") != 0) {
		std::cerr << "Error";
	}
	return true;
}

//Funkcja wypo¿ycza film. Tworzy datê oddania na bazie aktualnej i konta klienta i dodaje rekord do BorrowedTitles.txt
int BorrowMovie(Client& CliObj, std::string& Movie) {
	if (CliObj.get_BorrowedNb() >= CliObj.get_MaxMovieNb()) {
		return 1;
	}

	std::ifstream InFile("MoviesList.txt");
	std::ofstream OutFile("temp.txt", std::ios_base::trunc);
	if (!InFile.is_open()) {
		return 1;
	}
	if (!OutFile.is_open()) {
		return 1;
	}
	int Available{ 0 };
	int LineCount{ 0 };
	std::string line;
	if (InFile.is_open()) {
		while (std::getline(InFile, line)) {
			if (LineCount % 2 == 0) {
				OutFile << line << '\n';
				if (line == Movie) {
					std::getline(InFile, line);
					try {
						Available = std::stoi(line);
					}
					catch (std::invalid_argument&) {
						InFile.close();
						OutFile.close();
						return 1;
					}
					if (Available > 0) {
						OutFile << (Available - 1) << '\n';
					}
					else {
						OutFile << Available << '\n';
					}

				}
			}
			else {
				OutFile << line << '\n';

			}
			LineCount++;
		}


		InFile.close();
		OutFile.close();
		if (std::remove("MoviesList.txt") != 0) {
			std::cerr << "Error";
		}

		if (std::rename("temp.txt", "MoviesList.txt") != 0) {
			std::cerr << "Error";
		}

	}
	if (Available > 0) {
		BorrowedStruct BorrowedMovie{};
		BorrowedMovie.Title = Movie;
		BorrowedMovie.BDate.Day = LocalTimeNow.tm_mday;
		BorrowedMovie.BDate.Month += LocalTimeNow.tm_mon;
		BorrowedMovie.BDate.Year += LocalTimeNow.tm_year;
		BorrowedMovie.RDate.Day = LocalTimeNow.tm_mday;
		BorrowedMovie.RDate.Month += LocalTimeNow.tm_mon;
		BorrowedMovie.RDate.Year += LocalTimeNow.tm_year;
		if (CliObj.get_ClientRank() == "Standard") {
			BorrowedMovie.RDate.Day += 7;
		}
		else if (CliObj.get_ClientRank() == "Premium") {
			BorrowedMovie.RDate.Day += 14;
		}
		if (BorrowedMovie.RDate.Month == 2) {
			if (BorrowedMovie.RDate.Year % 4 != 0) { //Not a LeapYear
				if (BorrowedMovie.RDate.Day > 28) {
					BorrowedMovie.RDate.Day -= 28;
					BorrowedMovie.RDate.Month++;
				}
			}
			else if (BorrowedMovie.RDate.Year % 100 != 0) { //LeapYear
				if (BorrowedMovie.RDate.Day > 29) {
					BorrowedMovie.RDate.Day -= 29;
					BorrowedMovie.RDate.Month++;
				}
			}
			else if (BorrowedMovie.RDate.Year % 400 != 0) { //Not a LeapYear
				if (BorrowedMovie.RDate.Day > 28) {
					BorrowedMovie.RDate.Day -= 28;
					BorrowedMovie.RDate.Month++;
				}
			}
			else {
				if (BorrowedMovie.RDate.Day > 29) { //LeapYear
					BorrowedMovie.RDate.Day -= 29;
					BorrowedMovie.RDate.Month++;
				}
			}
		}
		else if (BorrowedMovie.RDate.Month == 4 || BorrowedMovie.RDate.Month == 6 || BorrowedMovie.RDate.Month == 9 || BorrowedMovie.RDate.Month == 11) { //30 days per month
			if (BorrowedMovie.RDate.Day > 30) {
				BorrowedMovie.RDate.Day -= 30;
				BorrowedMovie.RDate.Month++;
			}
		}
		else if (BorrowedMovie.RDate.Month == 12) { //End of the year
			if (BorrowedMovie.RDate.Day > 31) {
				BorrowedMovie.RDate.Day -= 31;
				BorrowedMovie.RDate.Month = 1;
				BorrowedMovie.RDate.Year++;
			}
		}
		else {
			if (BorrowedMovie.RDate.Day > 31) { //31 days per month
				BorrowedMovie.RDate.Day -= 31;
				BorrowedMovie.RDate.Month++;
			}
		}
		CliObj.add_BorrowedTitle(BorrowedMovie);
		return 0;
	}
	else {
		//std::cout << "No copies are available" << std::endl;
		return 2;
	}
}

//Funkcja tworz¹ca nowe konto klienta
void CreateClientAccount(void) {
	std::ifstream InFile("ClientsList.txt");
	std::string line;
	std::string Login;
	int RecordFound{ 0 };
	int LineCount{ 0 };

	if (!InFile.is_open()) {
		return;
	}

	while (1) {
		std::cout << "Write your account login: ";
		std::getline(std::cin, Login);
		if (InFile.is_open()) {
			while (std::getline(InFile, line)) {
				if (LineCount % 2 == 0) {
					if (line == Login) {
						RecordFound = 1;
						break;
					}
				}
				LineCount++;
			}
		}
		if (RecordFound == 0) {
			break;
		}
		else {
			std::cout << "This login is already in use. Please try something else." << std::endl;
			RecordFound = 0;
			InFile.seekg(0, std::ios::beg);
			continue;
		}
	}
	InFile.close();

	std::string Password;
	std::cout << "Write your password: ";
	std::getline(std::cin, Password);
	std::string Name;
	std::cout << "Write your name: ";
	std::getline(std::cin, Name);
	std::string Surname;
	std::cout << "Write your surname: ";
	std::getline(std::cin, Surname);
	int PhoneNb{ 0 };
	std::cout << "Enter your phone number: ";
	std::getline(std::cin, line);
	try {
		PhoneNb = std::stoi(line);
	}
	catch (std::invalid_argument&) {
		PhoneNb = 123456789;
	}
	std::string eMail;
	std::cout << "Enter your eMail address: ";
	std::getline(std::cin, eMail);

	Client obj(Login, Password, Name, Surname, PhoneNb, eMail);

	SaveNewAccount(obj);
}

//Funkcja zapisuj¹ca nowo utworzone konto do plików
void SaveNewAccount(const Client& obj) {

	std::ofstream OutFile("Clients.txt", std::ios_base::app);
	std::ofstream OutFileList("ClientsList.txt", std::ios_base::app);

	if (!OutFile.is_open()) {
		return;
	}
	if (!OutFileList.is_open()) {
		return;
	}

	if (OutFileList.is_open()) {
		OutFileList << obj.get_Login() << '\n';
		OutFileList << obj.get_Password() << '\n';
	}

	if (OutFile.is_open()) {
		OutFile << obj.get_Login() << '\n';
		OutFile << obj.get_Password() << '\n';
		OutFile << obj.get_UserName() << '\n';
		OutFile << obj.get_UserSurname() << '\n';
		OutFile << obj.get_CreationDate().Day << '\n';
		OutFile << obj.get_CreationDate().Month << '\n';
		OutFile << obj.get_CreationDate().Year << '\n';
		OutFile << obj.get_PhoneNb() << '\n';
		OutFile << obj.get_eMail() << '\n';
		OutFile << obj.get_MaxMovieNb() << '\n';
		OutFile << obj.get_BorrowedNb() << '\n';
		OutFile << obj.get_ClientRank() << '\n';
	}
	OutFile.close();
}

//Funkcja do zapisywania nowego has³a klienta po zmianach
void SaveNewClientPass(const Client& obj) {
	std::ifstream InFile("ClientsList.txt");
	std::ofstream OutFile("temp.txt", std::ios::trunc);

	int LineCount{ 0 };
	std::string line;

	if (InFile.is_open()) {
		while (std::getline(InFile, line)) {
			if (LineCount % 2 == 0) {
				if (line == obj.get_Login()) {
					OutFile << line << '\n';
					OutFile << obj.get_Password() << '\n';
					std::getline(InFile, line);
					LineCount++;
				}
				else {
					OutFile << line << '\n';
				}
			}
			else {
				OutFile << line << '\n';
			}
			LineCount++;
		}
	}

	InFile.close();
	OutFile.close();

	if (std::remove("ClientsList.txt") != 0) {
		std::cerr << "Error";
	}

	if (std::rename("temp.txt", "ClientsList.txt") != 0) {
		std::cerr << "Error";
	}
	return;
}

//Funkcja do zapisu konta klienta po zmianach
void SaveAccount(const Client& obj, const std::string& key) {

	std::string line;
	std::ifstream InFile("Clients.txt");
	std::ofstream OutFile("temp.txt", std::ios_base::trunc);

	if (!InFile.is_open()) {
		return;
	}
	if (!OutFile.is_open()) {
		return;
	}

	while (std::getline(InFile, line)) {
		if (line == key) {
			OutFile << obj.get_Login() << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_Password() << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_UserName() << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_UserSurname() << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_CreationDate().Day << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_CreationDate().Month << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_CreationDate().Year << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_PhoneNb() << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_eMail() << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_MaxMovieNb() << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_BorrowedNb() << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_ClientRank() << '\n';
		}
		else {
			OutFile << line << '\n';
		}
	}
	InFile.close();
	OutFile.close();

	if (std::remove("Clients.txt") != 0) {
		std::cerr << "Error";
	}

	if (std::rename("temp.txt", "Clients.txt") != 0) {
		std::cerr << "Error";
	}
}

//Funkcja poszukuj¹ca konto klienta i sprawdzaj¹ca podane has³o do konta
int FindClientAccount(const std::string key, const std::string pass) {
	std::ifstream InFile("ClientsList.txt");
	int LineCount{ 0 };
	std::string line;

	if (InFile.is_open()) {
		while (std::getline(InFile, line)) {
			if (LineCount % 2 == 0) {
				if (line == key) {
					std::getline(InFile, line);
					if (line == pass) {
						InFile.close();
						return 0;
					}
					else {
						//std::cout << "Wrong password.";
						InFile.close();
						return 1;
					}
				}
			}
			LineCount++;
		}
		//std::cout << "Couldn't find such account.";
		InFile.close();
		return 2;
	}
	InFile.close();
	return 3;
}

//Funkcja sprawdzaj¹ca istnienie konta klienta w bazie
bool FindClientAccount(const std::string key) {
	std::ifstream InFile("ClientsList.txt");
	int LineCount{ 0 };
	std::string line;

	if (!InFile.is_open()) {
		return false;
	}

	while (std::getline(InFile, line)) {
		if (LineCount % 2 == 0) {
			if (line == key) {
				InFile.close();
				return true;
			}
		}
		LineCount++;
	}
	InFile.close();
	return false;
}

//Funkcja ³aduj¹ca dane klienta do obiektu klasy klient na podstawie loginu
void LoadAccount(const std::string key, std::shared_ptr<Client>& InObj) {
	std::string Login;
	std::string Password;
	std::string UserName;
	std::string UserSurname;
	std::string eMail;
	std::string ClientRank;

	int PhoneNb{};
	int MaxMovieNb{};
	int BorrowedNb{};

	DateStruct CreationDate;

	int LineCount{ 0 };
	std::string line;
	std::ifstream InFile("Clients.txt");

	if (!InFile.is_open()) {
		return;
	}

	while (std::getline(InFile, line)) {
		if (LineCount % 12 == 0) {
			if (line == key) {
				Login = line;
				std::getline(InFile, line);
				Password = line;
				std::getline(InFile, line);
				UserName = line;
				std::getline(InFile, line);
				UserSurname = line;
				std::getline(InFile, line);
				CreationDate.Day = std::stoi(line);
				std::getline(InFile, line);
				CreationDate.Month = std::stoi(line);
				std::getline(InFile, line);
				CreationDate.Year = std::stoi(line);
				std::getline(InFile, line);
				PhoneNb = std::stoi(line);
				std::getline(InFile, line);
				eMail = line;
				std::getline(InFile, line);
				MaxMovieNb = std::stoi(line);
				std::getline(InFile, line);
				BorrowedNb = std::stoi(line);
				std::getline(InFile, line);
				ClientRank = line;
			}
		}
		LineCount++;
	}
	InFile.close();
	InObj = std::make_shared<Client>(Login, Password, UserName, UserSurname, PhoneNb, eMail, MaxMovieNb, BorrowedNb, ClientRank, CreationDate);
	return;
}

//Funkcja zapisuj¹ca nowo utworzone konto admina do plików
void SaveNewAccount(const Admin& obj) {

	std::ofstream OutFile("Admins.txt", std::ios_base::app);
	std::ofstream OutFileList("AdminsList.txt", std::ios_base::app);

	if (OutFileList.is_open()) {
		OutFileList << obj.get_Login() << '\n';
		OutFileList << obj.get_Password() << '\n';
	}
	OutFileList.close();

	if (OutFile.is_open()) {
		OutFile << obj.get_Login() << '\n';
		OutFile << obj.get_Password() << '\n';
		OutFile << obj.get_UserName() << '\n';
		OutFile << obj.get_UserSurname() << '\n';
		OutFile << obj.get_CreationDate().Day << '\n';
		OutFile << obj.get_CreationDate().Month << '\n';
		OutFile << obj.get_CreationDate().Year << '\n';
		OutFile << obj.get_PhoneNb() << '\n';
		OutFile << obj.get_eMail() << '\n';
		OutFile << obj.get_EmployeeTitle() << '\n';
	}
	OutFile.close();
}

//Funkcja tworz¹ca nowe konto administratora
void CreateAdminAccount(void) {
	std::ifstream InFile("AdminsList.txt");
	std::string line;
	std::string Login;
	int RecordFound{ 0 };
	int LineCount{ 0 };

	if (!InFile.is_open()) {
		return;
	}

	while (1) {
		std::cout << "Write your account login: ";
		std::getline(std::cin, Login);
		if (InFile.is_open()) {
			while (std::getline(InFile, line)) {
				if (LineCount % 2 == 0) {
					if (line == Login) {
						RecordFound = 1;
						break;
					}
				}
				LineCount++;
			}
		}
		if (RecordFound == 0) {
			break;
		}
		else {
			std::cout << "This login is already in use. Please try something else." << std::endl;
			RecordFound = 0;
			InFile.seekg(0, std::ios::beg);
			continue;
		}
	}
	InFile.close();

	std::string Password;
	std::cout << "Write your password: ";
	std::getline(std::cin, Password);
	std::string Name;
	std::cout << "Write your name: ";
	std::getline(std::cin, Name);
	std::string Surname;
	std::cout << "Write your surname: ";
	std::getline(std::cin, Surname);
	int PhoneNb{ 0 };
	std::cout << "Enter your phone number: ";
	std::getline(std::cin, line);
	try {
		PhoneNb = std::stoi(line);
	}
	catch (std::invalid_argument&) {
		PhoneNb = 123456789;
	}
	std::string eMail;
	std::cout << "Enter your eMail address: ";
	std::getline(std::cin, eMail);
	std::string EmployeeTitle;
	std::cout << "Enter your employee title: ";
	std::getline(std::cin, EmployeeTitle);
	Admin obj(Login, Password, Name, Surname, PhoneNb, eMail, EmployeeTitle);

	SaveNewAccount(obj);
}

//Funkcja do zapisu konta admina po zmianach
void SaveAccount(Admin& obj, const std::string& key) {

	int LineCount{ 0 };
	std::string line;

	std::ifstream InFile("Admins.txt");
	std::ofstream OutFile("temp.txt", std::ios_base::trunc);

	if (!InFile.is_open()) {
		return;
	}
	if (!OutFile.is_open()) {
		return;
	}

	while (std::getline(InFile, line)) {
		if (line == key) {
			OutFile << obj.get_Login() << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_Password() << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_UserName() << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_UserSurname() << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_CreationDate().Day << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_CreationDate().Month << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_CreationDate().Year << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_PhoneNb() << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_eMail() << '\n';
			std::getline(InFile, line);
			OutFile << obj.get_EmployeeTitle() << '\n';
		}
		else {
			OutFile << line << '\n';
		}
		LineCount++;
	}
	InFile.close();
	OutFile.close();

	if (std::remove("Admins.txt") != 0) {
		std::cerr << "Error";
	}

	if (std::rename("temp.txt", "Admins.txt") != 0) {
		std::cerr << "Error";
	}


}

//Funkcja poszukuj¹ca konto admina i sprawdzaj¹ca podane has³o do konta
int FindAdminAccount(const std::string key, const std::string pass) {
	std::ifstream InFile("AdminsList.txt");
	int LineCount{ 0 };
	std::string line;

	if (InFile.is_open()) {
		while (std::getline(InFile, line)) {
			if (LineCount % 2 == 0) {
				if (line == key) {
					std::getline(InFile, line);
					if (line == pass) {
						InFile.close();
						return 0;
					}
					else {
						std::cout << "Wrong password." << std::endl;
						InFile.close();
						return 1;
					}
				}
			}
			LineCount++;
		}
		std::cout << "Couldn't find such account." << std::endl;
		InFile.close();
		return 2;
	}
	InFile.close();
	return 3;
}

//Funkcja ³aduj¹ca dane admina do obiektu klasy Admin na podstawie loginu
void LoadAccount(const std::string key, std::shared_ptr<Admin>& InObj) {
	std::string Login;
	std::string Password;
	std::string UserName;
	std::string UserSurname;
	std::string eMail;
	std::string EmployeeTitle;

	int PhoneNb{ 0 };

	DateStruct CreationDate;

	int LineCount{ 0 };
	std::string line;
	std::ifstream InFile("Admins.txt");
	if (!InFile.is_open()) {
		return;
	}

	while (std::getline(InFile, line)) {
		if (LineCount % 10 == 0) {
			if (line == key) {
				Login = line;
				std::getline(InFile, line);
				Password = line;
				std::getline(InFile, line);
				UserName = line;
				std::getline(InFile, line);
				UserSurname = line;
				std::getline(InFile, line);
				try {
					CreationDate.Day = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFile.close();
					return;
				}
				std::getline(InFile, line);
				try {
					CreationDate.Month = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFile.close();
					return;
				}
				std::getline(InFile, line);
				try {
					CreationDate.Year = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFile.close();
					return;
				}
				std::getline(InFile, line);
				try {
					PhoneNb = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFile.close();
					return;
				}
				std::getline(InFile, line);
				eMail = line;
				std::getline(InFile, line);
				EmployeeTitle = line;
			}
		}
		LineCount++;
	}
	InFile.close();
	InObj = std::make_shared<Admin>(Login, Password, UserName, UserSurname, PhoneNb, eMail, EmployeeTitle, CreationDate);
	return;
}

//Funkcja ³aduj¹ca dane filmu do obiektu klasy Movie na bazie tytu³u
void LoadMovie(const std::string key, std::shared_ptr<Movie>& InObj) {
	std::string MovieTitle;
	std::string Genre;
	std::string Director;
	DateStruct ReleaseDate;
	TimeStruct MovieLength;
	std::vector<std::string> MainActors;
	std::string MovieInfo;
	int AmountInStore;
	int AmountAvailable;

	std::string line;
	std::string ActorName;
	std::string ActorSurname;

	int LineCount{ 0 };

	std::ifstream InFileObj("Movies.txt");
	std::ifstream InFileNb("MoviesList.txt");
	if (!InFileNb.is_open()) {
		return;
	}
	if (!InFileObj.is_open()) {
		return;
	}

	while (std::getline(InFileObj, line)) {
		if (LineCount % 11 == 0) {
			if (line == key) {
				MovieTitle = line;
				std::getline(InFileObj, line);
				Genre = line;
				std::getline(InFileObj, line);
				Director = line;
				std::getline(InFileObj, line);
				try {
					ReleaseDate.Day = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFileNb.close();
					InFileObj.close();
					return;
				}
				std::getline(InFileObj, line);
				try {
					ReleaseDate.Month = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFileNb.close();
					InFileObj.close();
					return;
				}
				std::getline(InFileObj, line);
				try {
					ReleaseDate.Year = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFileNb.close();
					InFileObj.close();
					return;
				}
				std::getline(InFileObj, line);
				try {
					MovieLength.Minute = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFileNb.close();
					InFileObj.close();
					return;
				}
				std::getline(InFileObj, line);
				try {
					MovieLength.Hour = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFileNb.close();
					InFileObj.close();
					return;
				}
				std::getline(InFileObj, line);
				std::istringstream token_stream(line);
				while (token_stream >> ActorName >> ActorSurname) {
					MainActors.push_back(ActorName + " " + ActorSurname);
				}
				std::getline(InFileObj, line);
				MovieInfo = line;
				std::getline(InFileObj, line);
				try {
					AmountInStore = std::stoi(line);
				}
				catch (std::invalid_argument&) {
					InFileNb.close();
					InFileObj.close();
					return;
				}
				break;
			}
		}
		LineCount++;
	}
	InFileObj.close();

	if (!InFileNb.is_open()) {
		std::cout << "Error" << std::endl;
	}

	LineCount = 0;
	while (std::getline(InFileNb, line)) {
		if (LineCount % 2 == 0) {
			if (line == key) {
				std::getline(InFileNb, line);
				AmountAvailable = std::stoi(line);
				break;
			}
		}
		LineCount++;
	}
	InFileNb.close();

	InObj = std::make_shared<Movie>(ReleaseDate, MovieLength, MainActors, MovieInfo, MovieTitle, Genre, Director, AmountInStore, AmountAvailable);
	return;
}

//Funkcja zapisuj¹ca nowy film do pliku
void SaveNewMovie(const Movie& obj) {

	std::ofstream OutFile("Movies.txt", std::ios_base::app);
	std::ofstream OutFileList("MoviesList.txt", std::ios_base::app);
	if (!OutFile.is_open()) {
		return;
	}
	if (!OutFileList.is_open()) {
		return;
	}

	if (OutFileList.is_open()) {
		OutFileList << obj.get_MovieTitle() << '\n';
		OutFileList << obj.get_AmountAvailable() << '\n';
	}
	OutFileList.close();
	if (OutFile.is_open()) {
		OutFile << obj.get_MovieTitle() << '\n';
		OutFile << obj.get_Genre() << '\n';
		OutFile << obj.get_Director() << '\n';
		OutFile << obj.get_ReleaseDate().Day << '\n';
		OutFile << obj.get_ReleaseDate().Month << '\n';
		OutFile << obj.get_ReleaseDate().Year << '\n';
		OutFile << obj.get_MovieLength().Minute << '\n';
		OutFile << obj.get_MovieLength().Hour << '\n';
		for (auto Act : obj.get_MainActors()) {
			OutFile << Act << " ";
		}
		OutFile << '\n';
		OutFile << obj.get_MovieInfo() << '\n';
		OutFile << obj.get_AmountInStore() << '\n';
	}
	OutFile.close();
}

//Funkcja tworz¹ca nowy obiekt Movie
void CreateMovie(void) {
	std::ifstream InFile("MoviesList.txt");
	std::string line;
	std::string Title;
	int RecordFound{ 0 };
	int LineCount{ 0 };
	if (!InFile.is_open()) {
		return;
	}

	while (1) {
		std::cout << "Write movie title: ";
		std::getline(std::cin, Title);
		if (InFile.is_open()) {
			while (std::getline(InFile, line)) {
				if (LineCount % 2 == 0) {
					if (line == Title) {
						RecordFound = 1;
						break;
					}
				}
				LineCount++;
			}
		}
		if (RecordFound == 0) {
			break;
		}
		else {
			std::cout << "This movie already exists." << std::endl;
			RecordFound = 0;
			InFile.seekg(0, std::ios::beg);
			continue;
		}
	}
	InFile.close();

	std::string Genre;
	std::cout << "Write movie genre: ";
	std::getline(std::cin, Genre);
	std::string Director;
	std::cout << "Write movie director: ";
	std::getline(std::cin, Director);
	DateStruct RelDate;
	std::cout << "Write release day: ";
	std::getline(std::cin, line);
	try {
		RelDate.Day = std::stoi(line);
	}
	catch (std::invalid_argument&) {
		return;
	}
	std::cout << "Write release month: ";
	std::getline(std::cin, line);
	try {
		RelDate.Month = std::stoi(line);
	}
	catch (std::invalid_argument&) {
		return;
	}
	std::cout << "Write release year: ";
	std::getline(std::cin, line);
	try {
		RelDate.Year = std::stoi(line);
	}
	catch (std::invalid_argument&) {
		return;
	}
	TimeStruct Length;
	std::cout << "Enter movie length minutes: ";
	std::getline(std::cin, line);
	try {
		Length.Minute = std::stoi(line);
	}
	catch (std::invalid_argument&) {
		return;
	}
	std::cout << "Enter movie length hours: ";
	std::getline(std::cin, line);
	try {
		Length.Hour = std::stoi(line);
	}
	catch (std::invalid_argument&) {
		return;
	}
	std::vector<std::string> Act;
	std::cout << "Enter main actors name and surname one by one. Enter \"skip\" when finished: ";
	while (std::getline(std::cin, line)) {
		if (line == "skip") {
			break;
		}
		else {
			Act.push_back(line);
		}
	}
	std::cout << "Write some description of this movie: ";
	std::string Info;
	std::getline(std::cin, Info);
	int InStore{};
	std::cout << "Enter amount in store: ";
	std::getline(std::cin, line);
	try {
		InStore = std::stoi(line);
	}
	catch (std::invalid_argument&) {
		return;
	}
	int Available{};
	std::cout << "Enter amount available: ";
	std::getline(std::cin, line);
	try {
		Available = std::stoi(line);
	}
	catch (std::invalid_argument&) {
		return;
	}

	Movie obj(RelDate, Length, Act, Info, Title, Genre, Director, InStore, Available);
	SaveNewMovie(obj);
}

//Funkcja do zampisu filmu po edycji
void SaveMovie(Movie& obj, const std::string& key) {

	std::string line;
	std::ifstream InFile("Movies.txt");
	std::ofstream OutFile("temp.txt", std::ios_base::trunc);
	if (!InFile.is_open()) {
		return;
	}
	if (!OutFile.is_open()) {
		return;
	}

	while (std::getline(InFile, line)) {
		if (line == key) {
			OutFile << obj.get_MovieTitle() << '\n';
			OutFile << obj.get_Genre() << '\n';
			OutFile << obj.get_Director() << '\n';
			OutFile << obj.get_ReleaseDate().Day << '\n';
			OutFile << obj.get_ReleaseDate().Month << '\n';
			OutFile << obj.get_ReleaseDate().Year << '\n';
			OutFile << obj.get_MovieLength().Minute << '\n';
			OutFile << obj.get_MovieLength().Hour << '\n';
			for (auto Act : obj.get_MainActors()) {
				OutFile << Act << " ";
			}
			OutFile << '\n';
			OutFile << obj.get_MovieInfo() << '\n';
			OutFile << obj.get_AmountInStore() << '\n';
			for (int i = 0; i < 10; i++) {
				std::getline(InFile, line);
			}
		}
		else {
			OutFile << line << '\n';
		}
	}
	InFile.close();
	OutFile.close();

	if (std::remove("Movies.txt") != 0) {
		std::cerr << "Error";
	}

	if (std::rename("temp.txt", "Movies.txt") != 0) {
		std::cerr << "Error";
	}

	InFile.clear();
	OutFile.clear();
	InFile = std::ifstream("MoviesList.txt");
	OutFile = std::ofstream("temp.txt", std::ios_base::trunc);
	if (!InFile.is_open()) {
		return;
	}
	if (!OutFile.is_open()) {
		return;
	}

	while (std::getline(InFile, line)) {
		if (line == key) {
			OutFile << obj.get_MovieTitle() << '\n';
			OutFile << obj.get_AmountAvailable() << '\n';
			std::getline(InFile, line);
		}
		else {
			OutFile << line << '\n';
		}
	}
	InFile.close();
	OutFile.close();

	if (std::remove("MoviesList.txt") != 0) {
		std::cerr << "Error";
	}

	if (std::rename("temp.txt", "MoviesList.txt") != 0) {
		std::cerr << "Error";
	}
}

//Funkcja sprawdzaj¹ca istnienie filmu o podanym tytule w bazie
bool FindMovie(std::string Title) {
	std::ifstream InFile("MoviesList.txt");
	std::string line;
	int LineCount{};
	if (!InFile.is_open()) {
		return false;
	}

	while (std::getline(InFile, line)) {
		if (LineCount % 2 == 0) {
			if (line == Title) {
				InFile.close();
				return true;
			}
		}
		LineCount++;
	}
	InFile.close();
	return false;
}

//Funkcja wyœwietlaj¹ca listê dostêpnych filmów
void PrintMoviesList(void) {
	std::ifstream InFile("MoviesList.txt");
	std::string line;
	if (!InFile.is_open()) {
		return;
	}

	while (std::getline(InFile, line)) {
		std::cout << "Title: " << line << std::endl;
		std::getline(InFile, line);
		std::cout << "Available: " << line << std::endl;
	}
	InFile.close();
}

//Funkcja porównuj¹ca dwie daty, zwraca true gdy Date2 > Date1
bool CompDate(const DateStruct& Date1, const DateStruct& Date2) {
	if (Date1.Year != Date2.Year) {
		return Date1.Year < Date2.Year;
	}
	if (Date1.Month != Date2.Month) {
		return Date1.Month < Date2.Month;
	}
	return Date1.Day < Date2.Day;
}

//Funkcja wyœwietlaj¹ca wypo¿yczone filmy i wypo¿yczaj¹cego
void PrintRented(void) {
	std::ifstream InFile("BorrowedTitles.txt");
	if (!InFile.is_open()) {
		return;
	}
	std::string line;
	while (std::getline(InFile, line)) {
		std::cout << "User: " << line << std::endl;
		std::getline(InFile, line);
		std::cout << "Title: " << line << std::endl;
		std::getline(InFile, line);
		std::cout << "Rent Date: " << line;
		std::getline(InFile, line);
		std::cout << ":" << line;
		std::getline(InFile, line);
		std::cout << ":" << line << std::endl;
		std::getline(InFile, line);
		std::cout << "Return Date: " << line;
		std::getline(InFile, line);
		std::cout << ":" << line;
		std::getline(InFile, line);
		std::cout << ":" << line << std::endl << std::endl;
	}
	InFile.close();
}

//Funkcja wyœwietlaj¹ca filmy nie oddane w terminie
void PrintOverdue(const DateStruct& CurrentDate) {
	std::ifstream InFile("BorrowedTitles.txt");

	if (!InFile.is_open()) {
		return;
	}

	std::string line, user;
	BorrowedStruct temp;
	while (std::getline(InFile, line)) {
		user = line;
		std::getline(InFile, line);
		temp.Title = line;
		std::getline(InFile, line);
		try {
			temp.BDate.Day = std::stoi(line);
		}
		catch (std::invalid_argument&) {
			InFile.close();
			return;
		}
		std::getline(InFile, line);
		try {
		temp.BDate.Month = std::stoi(line);
		}
		catch (std::invalid_argument&) {
			InFile.close();
			return;
		}
		std::getline(InFile, line);
		try {
			temp.BDate.Year = std::stoi(line);
		}
		catch (std::invalid_argument&) {
			InFile.close();
			return;
		}
		std::getline(InFile, line);
		try {
			temp.RDate.Day = std::stoi(line);
		}
		catch (std::invalid_argument&) {
			InFile.close();
			return;
		}
		std::getline(InFile, line);
		try {
			temp.RDate.Month = std::stoi(line);
		}
		catch (std::invalid_argument&) {
			InFile.close();
			return;
		}
		std::getline(InFile, line);
		try {
			temp.RDate.Year = std::stoi(line);
		}
		catch (std::invalid_argument&) {
			InFile.close();
			return;
		}
		if (CompDate(temp.RDate, CurrentDate)) {
			std::cout << "User: " << user << std::endl;
			std::cout << temp;
		}
	}
	InFile.close();
}