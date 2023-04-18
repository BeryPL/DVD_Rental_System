#include "Structs.h"
#include <iostream>

TimeStruct::TimeStruct(int H, int M) : Hour{ H }, Minute{ M } {}

std::ostream& operator<< (std::ostream& out, TimeStruct Time) {
	out << Time.Hour;
	out << ":";
	out << Time.Minute;
	out << std::endl;
	return out;
}

DateStruct::DateStruct(int Y, int M, int D) : Year{ Y }, Month{ M }, Day{ D } {}

std::ostream& operator<< (std::ostream& out, DateStruct Date) {
	out << Date.Day;
	out << ":";
	out << Date.Month;
	out << ":";
	out << Date.Year;
	out << std::endl;
	return out;
}

BorrowedStruct::BorrowedStruct(DateStruct B, DateStruct R, std::string T) : Title{ T }, BDate{ B }, RDate{ R } {}

std::ostream& operator<< (std::ostream& out, BorrowedStruct& obj) {
	out << "Title: " << obj.Title << std::endl;
	out << "Borrow Date: " << obj.BDate;
	out << "Return Date: " << obj.RDate;
	return out;
}