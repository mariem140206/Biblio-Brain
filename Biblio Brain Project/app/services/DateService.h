#pragma once
#include <string>
using namespace std;

class DateService {
public:
    static string getCurrentDate();
    static string addDays(const string& date, int days);
};
