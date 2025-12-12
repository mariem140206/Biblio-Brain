#pragma once
#include <string>
#include "../../core/BaseModel.hpp"
#include "json.hpp" 

using std::string;
using std::vector;
using namespace std;

using json = nlohmann::json;

struct User {
 int id;
 string name;
 string email;
 string password;
 string role; // admin, librarian, user
 string phoneNumber;
 string dateOfBirth;

 User() : id(-1), name(""), email(""), password(""), role("user"), phoneNumber(""), dateOfBirth("") {}

 static User fromJson(const json& j);
 json toJson() const;
 bool isValid() const { return id != -1 && !email.empty(); }
};

class UserModel : public BaseModel<User> {
public:
 UserModel();
 void create( User& user);
 User findByEmail(const string& email);
 User getUserById(int id);
 vector<User> search(const string& query);
 private:
 string toLower(string s);
}; 