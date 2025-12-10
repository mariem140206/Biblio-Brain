
#pragma once
#include "../../core/BaseModel.hpp"
#include <string>
 #include <algorithm>

using namespace std;

struct User {
    int id;
    string name;
    string email;
    string password;
    string role;
    string phoneNumber;
    string dateOfBirth;

    User();
    static User fromJson(const json &j);
    json toJson() const;
    bool isValid() const;
};

class UserModel : public BaseModel<User> {
public:
    UserModel();

    void create(const User &user);
    vector<User> search(const string &query);
    int login(const string &email, const string &password);
    User getUserById(int id);

private:
    string toLower(string s);
};
