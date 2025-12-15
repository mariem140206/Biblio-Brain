#include "DateService.h"
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;
string DateService::getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    stringstream ss;
    ss << setfill('0')
       << setw(4) << (1900 + ltm->tm_year) << "-"
       << setw(2) << (1 + ltm->tm_mon) << "-"
       << setw(2) << ltm->tm_mday;
    return ss.str();
}
string DateService::addDays(const string& date, int days) {
    tm ltm = {};
    sscanf(date.c_str(), "%d-%d-%d",
           &ltm.tm_year, &ltm.tm_mon, &ltm.tm_mday);

    ltm.tm_year -= 1900;
    ltm.tm_mon  -= 1;

    time_t timeValue = mktime(&ltm);
    timeValue += days * 24 * 60 * 60;
    tm* newDate = localtime(&timeValue);

    stringstream ss;
    ss << setfill('0')
       << setw(4) << (1900 + newDate->tm_year) << "-"
       << setw(2) << (1 + newDate->tm_mon + 1) << "-"
       << setw(2) << newDate->tm_mday;

    return ss.str();
}
