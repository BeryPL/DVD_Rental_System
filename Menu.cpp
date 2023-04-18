#include "Menu.h"

void MainMenu(void) {
	system("cls");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	std::cout << std::setfill('-');
	std::cout << std::setw(csbi.srWindow.Right - csbi.srWindow.Left + 1) << " " << std::endl;  //Do tego momentu czyœci ekran i tworzy podkreslenie przec caly obszar terminala
	std::cout << "Welcome to DVD movies rental DataBase program!!!" << std::endl << std::endl << std::endl;
	std::cout << "Enter number to choose desired option:" << std::endl;
	std::cout << "1. User login." << std::endl;
	std::cout << "2. Admin login." << std::endl;
	std::cout << "3. Create account." << std::endl;
	std::cout << "4. Exit program." << std::endl;
}

void refresh(void) {
	system("cls");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	std::cout << std::setfill('-');
	std::cout << std::setw(csbi.srWindow.Right - csbi.srWindow.Left + 1) << " " << std::endl;
	std::cout << std::endl << std::endl;
}

void ClientMenu(int& MenuOption) {
	int error{ 0 };
	std::string Login;
	std::string Password;
	while (1) {  //Pêtla logowania konta klienta
		refresh();
		std::cout << "Enter user login. Write \"exit\" to exit this submenu." << std::endl;
		std::cout << "Login: ";
		std::cin >> Login;
		if (Login == "exit") {
			error = 1;
			break;
		}
		refresh();
		int FindState{};
		int d{ 3 };
		while (1) {  //Pod pêtla logowania obs³uguj¹ca ograniczon¹ iloœæ prób logowania oraz nieprawid³owy login
			refresh();
			std::cout << "Enter user password. Chances left - " << d << std::endl;
			std::cin >> Password;
			FindState = FindClientAccount(Login, Password);
			if (FindState == 1) {
				d--;
				if (d == 0) {
					refresh();
					std::cout << "Wrong again. No more chances left. Press \"Enter\" to continue." << std::endl;
					std::cin.ignore();
					std::cin.ignore();
					error = 1;
					break;
				}
				refresh();
				std::cout << "Wrong password. Try again.";
				continue;
			}
			else if (FindState == 2) {
				refresh();
				std::cout << "No such account. Press \"Enter\" to continue." << std::endl;
				std::cin.ignore();
				std::cin.ignore();
				error = 0;
				break;
			}
			else if (FindState == 0) {
				error = 0;
				break;
			}
		}
		if (error == 1 || ((error == 0) && (FindState == 0))) {
			break;
		}

	}
	if (error == 1) {
		return;
	}
	std::shared_ptr<Client> CliObj;  //Utworzenie pamiêci dla konta klienta i za³adowanie danych konta z pliku
	LoadAccount(Login, CliObj);
	CliObj->LoadBorrowedTitles();
	std::string UserInput;
	std::string Menu;
	int Logout{ 0 };
	while (1) {  //Pêtla g³ówna menu obs³ugi klienta
		refresh();
		std::cout << "Enter number to choose desired option:" << std::endl;
		std::cout << "1. Print movies list." << std::endl;
		std::cout << "2. Print movie info." << std::endl;
		std::cout << "3. Print my currently borrowed movies." << std::endl;
		std::cout << "4. Borrow movie." << std::endl;
		std::cout << "5. Return movie." << std::endl;
		std::cout << "6. Account info." << std::endl;
		std::cout << "7. Logout." << std::endl;
		std::getline(std::cin, Menu);
		try {
			MenuOption = std::stoi(Menu);
		}
		catch (std::invalid_argument&) {
			continue;
		}
		switch (MenuOption) {
		case 1: //Prints movies list
			refresh();
			PrintMoviesList();
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 2: //Prints movie info
			refresh();
			std::cout << "Enter title: ";
			std::getline(std::cin, UserInput);
			if (FindMovie(UserInput)) {
				std::shared_ptr<Movie> MovObj;
				LoadMovie(UserInput, MovObj);
				refresh();
				std::cout << *MovObj;
			}
			else {
				refresh();
				std::cout << "Sorry. We do not have this movie." << std::endl;
			}
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 3:  //Print my currently borrowed movies.
			refresh();
			for (auto mov : CliObj->get_BorrowedTitles()) {
				std::cout << "Title: " << mov.Title << std::endl;
				std::cout << "Rental date - " << mov.BDate;
				std::cout << "Return date - " << mov.RDate;
			}
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 4:  //Borrow movie.
			refresh();
			std::cout << "Enter movie title to rent: ";
			std::getline(std::cin, UserInput);
			if (FindMovie(UserInput)) {
				int Rent{};
				Rent = BorrowMovie(*CliObj, UserInput);
				if (Rent == 0) {
					refresh();
					std::cout << "Movie rented succesfully." << std::endl;
					std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
					std::cin.ignore();
					break;
				}
				else if (Rent == 1) {
					refresh();
					std::cout << "You have reached your renting limit. Please return something.";
					std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
					std::cin.ignore();
					break;
				}
				else if (Rent == 2) {
					refresh();
					std::cout << "Sorry. But we do not have any copies of this movie available for rental.";
					std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
					std::cin.ignore();
					break;
				}
			}
			else {
				refresh();
				std::cout << "We do not rent this movie or you provided wrong title.";
				std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
				std::cin.ignore();
				break;
			}
		case 5:  //Return movie.
			refresh();
			std::cout << "Enter movie title to return: ";
			std::getline(std::cin, UserInput);
			if (ReturnMovie(*CliObj, UserInput)) {
				refresh();
				std::cout << "Movie returned.";
				std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
				std::cin.ignore();
				break;
			}
			else {
				refresh();
				std::cout << "Movie not returned, since we can not find it on your account.";
				std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
				std::cin.ignore();
				break;
			}
		case 6:  //Account info.
			refresh();
			std::cout << *CliObj;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 7:  //Saves changes to file and loging out client
			refresh();
			CliObj->SaveBorrowedTitles();
			SaveAccount(*CliObj, CliObj->get_Login());
			CliObj.~shared_ptr();
			Logout = 1;
			std::cout << "Logged out.";
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		default:
			break;

		}
		if (Logout == 1) {
			break;
		}
	}
}

void AdminMenu(int& MenuOption) {
	int error{ 0 };
	std::string Login;
	std::string Password;
	while (1) {  //Pêtla g³ówna logowania konta administratora
		refresh();
		std::cout << "Enter admin login. Write \"exit\" to exit this submenu." << std::endl;
		std::cout << "Login: ";
		std::cin >> Login;
		if (Login == "exit") {
			error = 1;
			break;
		}
		refresh();
		int FindState{};
		int d{ 3 };
		while (1) {  //Pod pêtla do obs³ugi ograniczonej iloœci prób logowania oraz nieprawid³owego loginu
			refresh();
			std::cout << "Enter admin password. Chances left - " << d << std::endl;
			std::cin >> Password;
			FindState = FindAdminAccount(Login, Password);
			if (FindState == 1) {
				d--;
				if (d == 0) {
					refresh();
					std::cout << "Wrong again. No more chances left. Press \"Enter\" to continue." << std::endl;
					std::cin.ignore();
					std::cin.ignore();
					error = 1;
					break;
				}
				refresh();
				std::cout << "Wrong password. Try again.";
				continue;
			}
			else if (FindState == 2) {
				refresh();
				std::cout << "No such account. Press \"Enter\" to continue." << std::endl;
				std::cin.ignore();
				std::cin.ignore();
				error = 0;
				break;
			}
			else if (FindState == 0) {
				error = 0;
				break;
			}
		}
		if (error == 1 || ((error == 0) && (FindState == 0))) {
			break;
		}

	}
	if (error == 1) {
		return;
	}
	std::shared_ptr<Admin> AdmObj;  //Utworzenie pamiêci dla potrzebnych obiektów oraz odczytanie konta administratora z pliku
	std::shared_ptr<Movie> MovObj;
	std::shared_ptr<Client> CliObj;
	LoadAccount(Login, AdmObj);
	std::string UserInput;
	std::string Menu;
	int Logout{ 0 };
	DateStruct CurrDate = { 1900 + LocalTimeNow.tm_year, 1 + LocalTimeNow.tm_mon, LocalTimeNow.tm_mday };
	while (1) {  //Pêtla g³ówna menu administratora systemu
		refresh();
		std::cout << "Enter number to choose desired option:" << std::endl;
		std::cout << "1. Print movies list." << std::endl;
		std::cout << "2. Print clients list." << std::endl;
		std::cout << "3. Print currently rented movies." << std::endl;
		std::cout << "4. Print overdue rented movies." << std::endl;
		std::cout << "5. Print  movie info." << std::endl;
		std::cout << "6. Print client account info." << std::endl;
		std::cout << "7. Add movie." << std::endl;
		std::cout << "8. Edit movie." << std::endl;
		std::cout << "9. Edit client account." << std::endl;
		std::cout << "10. Account info." << std::endl;
		std::cout << "11. Logout." << std::endl;
		std::getline(std::cin, Menu);
		try {
			MenuOption = std::stoi(Menu);
		}
		catch (std::invalid_argument&) {
			continue;
		}
		switch (MenuOption) {
		case 1:  //Print movies list
			refresh();
			PrintMoviesList();
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 2:  //Print clients list
			refresh();
			PrintClients();
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 3:  //Print currently rented movies
			refresh();
			PrintRented();
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 4:  //Print overdue rented movies.
			refresh();
			PrintOverdue(CurrDate);
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 5:  //Print  movie info.
			refresh();
			std::cout << "Enter title: ";
			std::getline(std::cin, UserInput);
			if (FindMovie(UserInput)) {
				std::shared_ptr<Movie> MovObj;
				LoadMovie(UserInput, MovObj);
				refresh();
				std::cout << *MovObj;
			}
			else {
				refresh();
				std::cout << "Sorry. We do not have this movie." << std::endl;
			}
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 6:  //Print client account info.
			refresh();
			std::cout << "Enter client account login: ";
			std::getline(std::cin, UserInput);
			if (FindClientAccount(UserInput)) {
				LoadAccount(UserInput, CliObj);
				CliObj->LoadBorrowedTitles();
				refresh();
				std::cout << *CliObj;
				CliObj.reset();
				std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
				std::cin.ignore();
				break;
			}
			else {
				refresh();
				std::cout << "No such account." << std::endl;
				std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
				std::cin.ignore();
				break;
			}
			break;
		case 7:  // Add movie
			refresh();
			CreateMovie();
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 8:  //Edit movie.
			refresh();
			std::cout << "Enter movie title: ";
			std::getline(std::cin, UserInput);
			if (FindMovie(UserInput)) {
				LoadMovie(UserInput, MovObj);
				refresh();
				MovieEditMenu(*MovObj);
			}
			break;
		case 9:  // Edit client account.
			refresh();
			std::cout << "Enter client login: ";
			std::getline(std::cin, UserInput);
			if (FindClientAccount(UserInput)) {
				LoadAccount(UserInput, CliObj);
				refresh();
				ClientEditMenu(*CliObj, *AdmObj);
			}
			break;
		case 10:  //Account info.
			refresh();
			std::cout << *AdmObj;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 11:  //Save and logout admin account
			refresh();
			SaveAccount(*AdmObj, AdmObj->get_Login());
			AdmObj.~shared_ptr();
			MovObj.~shared_ptr();
			CliObj.~shared_ptr();
			Logout = 1;
			std::cout << "Logged out.";
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		default:
			break;
		}
		if (Logout == 1) {
			break;
		}
	}
}

void ClientEditMenu(Client& CliObj, Admin& AdmObj) {
	std::string UserInput;
	int Exit{ 0 };
	int MenuOption{ 0 };
	int UserInt{};
	std::string Menu;
	DateStruct tempDate;
	TimeStruct tempTime;
	std::vector<std::string> Actors;
	while (1) {  //Pêtla g³ówna menu edycji parametrów klienta
		refresh();
		std::cout << "Enter number to edit desired client option:" << std::endl;
		std::cout << "1. Name." << std::endl;
		std::cout << "2. Surname." << std::endl;
		std::cout << "3. Reset password." << std::endl;
		std::cout << "4. Email." << std::endl;
		std::cout << "5. Phone number." << std::endl;
		std::cout << "6. Client info." << std::endl;
		std::cout << "7. Client rank." << std::endl;
		std::cout << "8. Save and exit." << std::endl;
		std::getline(std::cin, Menu);
		try {
			MenuOption = std::stoi(Menu);
		}
		catch (std::invalid_argument&) {
			continue;
		}
		switch (MenuOption) {
		case 1: //Change name
			refresh();
			std::cout << "Current name: " << CliObj.get_UserName() << std::endl;
			std::cout << "Enter new name: ";
			std::getline(std::cin, UserInput);
			AdmObj.set_ClientName(CliObj, UserInput);
			refresh();
			std::cout << "Name changed." << std::endl;
			std::cout << "New Name: " << CliObj.get_UserName() << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 2:  //Change surname
			refresh();
			std::cout << "Current surname: " << CliObj.get_UserSurname() << std::endl;
			std::cout << "Enter new surname: ";
			std::getline(std::cin, UserInput);
			AdmObj.set_ClientSurname(CliObj, UserInput);
			refresh();
			std::cout << "Surname changed." << std::endl;
			std::cout << "New Surname: " << CliObj.get_UserSurname() << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 3:  //Set new passwoed for client
			refresh();
			std::cout << "Enter new password. Old password will be overwritten: ";
			std::getline(std::cin, UserInput);
			AdmObj.set_ClientPassword(CliObj, UserInput);
			refresh();
			std::cout << "Password changed." << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();				break;
		case 4:  //Change email address
			refresh();
			std::cout << "Current email: " << CliObj.get_eMail() << std::endl;
			std::cout << "Enter new email: ";
			std::getline(std::cin, UserInput);
			AdmObj.set_ClientrEmail(CliObj, UserInput);
			refresh();
			std::cout << "Email changed." << std::endl;
			std::cout << "New Email: " << CliObj.get_eMail() << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();				break;
		case 5:  //Change clients phone number
			refresh();
			std::cout << "Current phone number: " << CliObj.get_PhoneNb() << std::endl;
			std::cout << "Enter new number: ";
			std::getline(std::cin, UserInput);
			try {
				UserInt = std::stoi(UserInput);
			}
			catch (std::invalid_argument&) {
				break;
			}
			AdmObj.set_ClientPhone(CliObj, UserInt);
			refresh();
			std::cout << "Phone changed." << std::endl;
			std::cout << "New phone: " << CliObj.get_PhoneNb() << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 6:  //Print client account info
			refresh();
			std::cout << CliObj << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 7:  //Change client rank
			refresh();
			std::cout << "Current rank: " << CliObj.get_ClientRank() << std::endl;
			std::cout << "Choose rank: " << std::endl;
			std::cout << "1. Standard - Max. movies to borrow: 10. Rent time: 7 days" << std::endl;
			std::cout << "2. Premium - Max. movies to borrow: 15. Rent time: 14 days" << std::endl;
			std::getline(std::cin, UserInput);
			try {
				UserInt = std::stoi(UserInput);
			}
			catch (std::invalid_argument&) {
				break;
			}
			AdmObj.set_ClientRank(CliObj, UserInt);
			refresh();
			if ((UserInt != 1) && (UserInt != 2)) {
				std::cout << "Wrong value. Rank not changed." << std::endl;
			}
			std::cout << "Rank changed." << std::endl;
			std::cout << "New rank: " << CliObj.get_ClientRank() << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 8:  //Save changes and/or exit
			refresh();
			std::cout << "Client object after changes:" << std::endl;
			std::cout << CliObj << std::endl;
			std::cout << "Do you want to save changes ? Y/N: ";
			while (std::getline(std::cin, UserInput)) {
				if (UserInput == "Y") {
					SaveAccount(CliObj, CliObj.get_Login());
					SaveNewClientPass(CliObj);
					Exit = 1;
					break;
				}
				else if (UserInput == "N") {
					Exit = 1;
					break;
				}
				continue;
			}
			break;
		default:
			break;
		}
		if (Exit == 1) {
			break;
		}
	}
}

void MovieEditMenu(Movie& MovObj) {
	std::string UserInput;
	std::string Menu;
	int Exit{ 0 };
	int MenuOption{ 0 };
	int UserInt{};
	DateStruct tempDate;
	TimeStruct tempTime;
	std::vector<std::string> Actors;
	while (1) {  //Pêtla g³ówna edycji parametrów filmu
		refresh();
		std::cout << "Enter number to edit desired movie option:" << std::endl;
		std::cout << "1. Genre." << std::endl;
		std::cout << "2. Director." << std::endl;
		std::cout << "3. Release date." << std::endl;
		std::cout << "4. Movie length." << std::endl;
		std::cout << "5. Change actors." << std::endl;
		std::cout << "6. Add actor." << std::endl;
		std::cout << "7. Amount in store." << std::endl;
		std::cout << "8. Amount available ." << std::endl;
		std::cout << "9. Movie info." << std::endl;
		std::cout << "10. Save and exit." << std::endl;
		std::getline(std::cin, Menu);
		try {
			MenuOption = std::stoi(Menu);
		}
		catch (std::invalid_argument&) {
			continue;
		}
		switch (MenuOption) {
		case 1:  //Change genre
			refresh();
			std::cout << "Current genre: " << MovObj.get_Genre() << std::endl;
			std::cout << "Enter new genre: ";
			std::getline(std::cin, UserInput);
			MovObj.set_Genre(UserInput);
			refresh();
			std::cout << "Genre changed." << std::endl;
			std::cout << "New genre: " << MovObj.get_Genre() << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 2:  //Change director
			refresh();
			std::cout << "Current director: " << MovObj.get_Director() << std::endl;
			std::cout << "Enter new director: ";
			std::getline(std::cin, UserInput);
			MovObj.set_Director(UserInput);
			refresh();
			std::cout << "Director changed." << std::endl;
			std::cout << "New director: " << MovObj.get_Director() << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 3: //Change release date
			refresh();
			std::cout << "Current release date: " << MovObj.get_ReleaseDate() << std::endl;
			std::cout << "Enter new date - day: ";
			std::getline(std::cin, UserInput);
			try {
				tempDate.Day = std::stoi(UserInput);
			}
			catch (std::invalid_argument&) {
				break;
			}
			std::cout << "Enter new date - month: ";
			std::getline(std::cin, UserInput);
			try {
				tempDate.Month = std::stoi(UserInput);
			}
			catch (std::invalid_argument&) {
				break;
			}
			std::cout << "Enter new date - year: ";
			std::getline(std::cin, UserInput);
			try {
				tempDate.Year = std::stoi(UserInput);
			}
			catch (std::invalid_argument&) {
				break;
			}
			MovObj.set_ReleaseDate(tempDate);
			refresh();
			std::cout << "Release date changed." << std::endl;
			std::cout << "New release date: " << MovObj.get_ReleaseDate() << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 4:  //Change movie length
			refresh();
			std::cout << "Current length: " << MovObj.get_MovieLength() << std::endl;
			std::cout << "Enter new length - minutes: ";
			std::getline(std::cin, UserInput);
			try {
				tempTime.Minute = std::stoi(UserInput);
			}
			catch (std::invalid_argument&) {
				break;
			}
			std::cout << "Enter new length - hours: ";
			std::getline(std::cin, UserInput);
			try {
				tempTime.Hour = std::stoi(UserInput);
			}
			catch (std::invalid_argument&) {
				break;
			}
			MovObj.set_MovieLength(tempTime);
			refresh();
			std::cout << "Movie length changed." << std::endl;
			std::cout << "New length: " << MovObj.get_MovieLength() << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 5:  //Change actors
			refresh();
			std::cout << "Current actors: ";
			for (auto Act : MovObj.get_MainActors()) {
				std::cout << Act << ", ";
			}
			std::cout << std::endl;
			std::cout << "Enter new actors. Enter \"skip\" to end: ";
			while (std::getline(std::cin, UserInput)) {
				if (UserInput != "skip") {
					Actors.push_back(UserInput);
				}
				else {
					break;
				}
			}
			MovObj.set_Actors(Actors);
			refresh();
			std::cout << "Movie actors changed." << std::endl;
			std::cout << "New actors: ";
			for (auto Act : MovObj.get_MainActors()) {
				std::cout << Act << ", ";
			}
			std::cout << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 6:  //Add actor
			refresh();
			std::cout << "Current actors: ";
			for (auto Act : MovObj.get_MainActors()) {
				std::cout << Act << ", ";
			}
			std::cout << std::endl;
			std::cout << "Add new actor. Enter \"skip\" to end: ";
			std::getline(std::cin, UserInput);
			if (UserInput != "skip") {
				MovObj.add_actor(UserInput);
			}
			else {
				break;
			}
			refresh();
			std::cout << "Actor added." << std::endl;
			std::cout << "New actors: ";
			for (auto Act : MovObj.get_MainActors()) {
				std::cout << Act << ", ";
			}
			std::cout << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 7:  //Change amount in store
			refresh();
			std::cout << "Current amount in store: " << MovObj.get_AmountInStore() << std::endl;
			std::cout << "Enter new amount: ";
			std::getline(std::cin, UserInput);
			try {
				UserInt = std::stoi(UserInput);
			}
			catch (std::invalid_argument&) {
				break;
			}
			MovObj.set_InStore(UserInt);
			refresh();
			std::cout << "Amount in store changed." << std::endl;
			std::cout << "New amount: " << MovObj.get_AmountInStore() << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 8:  //Change amount available for renting
			refresh();
			std::cout << "Current amount available: " << MovObj.get_AmountAvailable() << std::endl;
			std::cout << "Enter new amount: ";
			std::getline(std::cin, UserInput);
			try {
				UserInt = std::stoi(UserInput);
			}
			catch (std::invalid_argument&) {
				break;
			}
			MovObj.set_Available(UserInt);
			refresh();
			std::cout << "Amount available changed." << std::endl;
			std::cout << "New amount: " << MovObj.get_AmountAvailable() << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 9:  //Change movie info
			refresh();
			std::cout << "Current movie info: " << MovObj.get_MovieInfo() << std::endl;
			std::cout << "Enter \"skip\" to end." << std::endl;
			std::cout << "Enter new info: ";
			std::getline(std::cin, UserInput);
			if (UserInput != "skip") {
				MovObj.set_MovieInfo(UserInput);
			}
			else {
				break;
			}
			refresh();
			std::cout << "Info changed." << std::endl;
			std::cout << "New info: " << MovObj.get_MovieInfo() << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 10:  //Save and/or exit
			refresh();
			std::cout << "Movie object after changes:" << std::endl;
			std::cout << MovObj << std::endl;
			std::cout << "Do you want to save changes ? Y/N: ";
			while (std::getline(std::cin, UserInput)) {
				if (UserInput == "Y") {
					SaveMovie(MovObj, MovObj.get_MovieTitle());
					Exit = 1;
					break;
				}
				else if (UserInput == "N") {
					Exit = 1;
					break;
				}
				continue;
			}
			break;
		default:
			break;
		}
		if (Exit == 1) {
			break;
		}
	}
}


void CreateMenu(int& MenuOption) {
	int Exit{ 0 };
	std::string UserInput;
	std::string Menu;
	while (1) {  //Pêtla g³ówna menu tworzenia kont
		refresh();
		std::cout << "Enter number to choose account type:" << std::endl;
		std::cout << "1. Create client account." << std::endl;
		std::cout << "2. Create admin account." << std::endl;
		std::cout << "3. Exit." << std::endl;
		std::getline(std::cin, Menu);
		try {
			MenuOption = std::stoi(Menu);
		}
		catch (std::invalid_argument&) {
			continue;
		}
		switch (MenuOption) {
		case 1:  //Create client account
			refresh();
			CreateClientAccount();
			std::cout << "Account created." << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 2:  //Create admin account
			refresh();
			CreateAdminAccount();
			std::cout << "Account created." << std::endl;
			std::cout << std::endl << "Press \"Enter\" to continue." << std::endl;
			std::cin.ignore();
			break;
		case 3:  //Exit
			Exit = 1;
			break;
		default:
			break;
		}
		if (Exit == 1) {
			break;
		}
	}
	return;
}