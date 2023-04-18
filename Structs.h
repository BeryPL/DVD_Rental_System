#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

//Struktura przechowuj�ca czas. Godzin� oraz minuty
struct TimeStruct {
	TimeStruct(int H = 0, int M = 0);
	int Hour{};
	int Minute{};
};

//Przeci��enie operatora wyj�cia w celu wy�wietlania struktury TimeStruct
std::ostream& operator<< (std::ostream& out, TimeStruct Time);

//Struktura przechowuj�ca dat�. Dzie�, miesi�c oraz rok.
struct DateStruct {
	DateStruct(int Y = 1900, int M = 1, int D = 0);
	int Year{};
	int Month{};
	int Day{};
};

//Przeci��enie operatora wyj�cia w celu wy�wietlania struktury DateStruct
std::ostream& operator<< (std::ostream& out, DateStruct Date);

//Struktura przechowuj�ca tytu� oraz dat� wypo�yczenia oraz zwrotu filmu.
struct BorrowedStruct {
	BorrowedStruct(DateStruct B = { 1900, 1, 0 }, DateStruct R = { 1900, 1, 0 }, std::string T = "none");
	std::string Title;
	DateStruct BDate;
	DateStruct RDate;
};

//Przeci��enie operatora wyj�cia w celu wy�wietlania struktury BorrowedStruct
std::ostream& operator<< (std::ostream& out, BorrowedStruct& obj);

#endif // !

