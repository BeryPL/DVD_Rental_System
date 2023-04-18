#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

//Struktura przechowuj¹ca czas. Godzinê oraz minuty
struct TimeStruct {
	TimeStruct(int H = 0, int M = 0);
	int Hour{};
	int Minute{};
};

//Przeci¹¿enie operatora wyjœcia w celu wyœwietlania struktury TimeStruct
std::ostream& operator<< (std::ostream& out, TimeStruct Time);

//Struktura przechowuj¹ca datê. Dzieñ, miesi¹c oraz rok.
struct DateStruct {
	DateStruct(int Y = 1900, int M = 1, int D = 0);
	int Year{};
	int Month{};
	int Day{};
};

//Przeci¹¿enie operatora wyjœcia w celu wyœwietlania struktury DateStruct
std::ostream& operator<< (std::ostream& out, DateStruct Date);

//Struktura przechowuj¹ca tytu³ oraz datê wypo¿yczenia oraz zwrotu filmu.
struct BorrowedStruct {
	BorrowedStruct(DateStruct B = { 1900, 1, 0 }, DateStruct R = { 1900, 1, 0 }, std::string T = "none");
	std::string Title;
	DateStruct BDate;
	DateStruct RDate;
};

//Przeci¹¿enie operatora wyjœcia w celu wyœwietlania struktury BorrowedStruct
std::ostream& operator<< (std::ostream& out, BorrowedStruct& obj);

#endif // !

