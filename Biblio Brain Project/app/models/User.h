#pragma once
#include <string>
#include "../../core/BaseModel.hpp"
#include "json.hpp" 

using json = nlohmann::json;

struct User {
 int id;
 std::string name;
 std::string email;
 std::string password;
 std::string role; // admin, librarian, user

 User() : id(-1), name(""), email(""), password(""), role("user") {}

 static User fromJson(const json& j);
 json toJson() const;
 bool isValid() const { return id != -1; }
};

class UserModel : public BaseModel<User> {
public:
 UserModel();
 void create(const User& user);
 User findByEmail(const std::string& email);
}; 