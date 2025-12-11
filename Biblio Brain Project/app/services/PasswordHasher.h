#pragma once
#include <string>
using namespace std;

class PasswordHasher {
public:
static string hash(const string& password);
static bool verify(const string& password, const string& hash);
};