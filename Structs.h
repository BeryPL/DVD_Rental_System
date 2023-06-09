#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

//Struktura przechowująca czas. Godzinę oraz minuty
struct TimeStruct {
	TimeStruct(int H = 0, int M = 0);
	int Hour{};
	int Minute{};
};

//Przeciążenie operatora wyjścia w celu wyświetlania struktury TimeStruct
std::ostream& operator<< (std::ostream& out, TimeStruct Time);

//Struktura przechowująca datę. Dzień, miesiąc oraz rok.
struct DateStruct {
	DateStruct(int Y = 1900, int M = 1, int D = 0);
	int Year{};
	int Month{};
	int Day{};
};

//Przeciążenie operatora wyjścia w celu wyświetlania struktury DateStruct
std::ostream& operator<< (std::ostream& out, DateStruct Date);

//Struktura przechowująca tytuł oraz datę wypożyczenia oraz zwrotu filmu.
struct BorrowedStruct {
	BorrowedStruct(DateStruct B = { 1900, 1, 0 }, DateStruct R = { 1900, 1, 0 }, std::string T = "none");
	std::string Title;
	DateStruct BDate;
	DateStruct RDate;
};

//Przeciążenie operatora wyjścia w celu wyświetlania struktury BorrowedStruct
std::ostream& operator<< (std::ostream& out, BorrowedStruct& obj);

#endif // !

