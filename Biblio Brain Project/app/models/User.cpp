#pragma once
#include "../../core/BaseModel.hpp"
#include <string>
#include <algorithm>
struct User {
    int id;
    string name;
    string email;
    string password;
    string role;          
    string phoneNumber;
    string dateOfBirth;

    User() : id(-1), name(""), email(""), password(""), role("client"), phoneNumber(""), dateOfBirth("") {}
 
    static User fromJson(const json &j) {
        User u;
        if(j.contains("id")) u.id = j["id"].get<int>();
        if(j.contains("name")) u.name = j["name"].get<string>();
        if(j.contains("email")) u.email = j["email"].get<string>();
        if(j.contains("password")) u.password = j["password"].get<string>();
        if(j.contains("role")) u.role = j["role"].get<string>();
        if(j.contains("phone_number")) u.phoneNumber = j["phone_number"].get<string>();
        if(j.contains("date_of_birth")) u.dateOfBirth = j["date_of_birth"].get<string>();
        return u;
    }

    json toJson() const {
        return {
            {"id", id},
            {"name", name},
            {"email", email},
            {"password", password},
            {"role", role},
            {"phone_number", phoneNumber},
            {"date_of_birth", dateOfBirth}
        };
    }


    bool isValid() const { return id != -1 && !email.empty(); }
};


class UserModel : public BaseModel<User> {
  public : 
    UserModel() : BaseModel("storage/users.json") {}

 
    void create(const User &user) {
        json data = getAllJson();
        data.push_back(user.toJson());
        data.back()["id"] = generateId(); 
        saveJson(data);
    }

    vector<User> search(const string &query) {
        vector<User> results;
        string lowerQuery = toLower(query);

        for(auto &item : all()) {
            if(toLower(item.name).find(lowerQuery) != string::npos || 
               toLower(item.email).find(lowerQuery) != string::npos ||
               toLower(item.phoneNumber).find(lowerQuery) != string::npos) {
                
                results.push_back(item);
            }
        }
        return results;
    }


   User findByEmail(const string& email){
        for (const auto& user : all()){
            if (user.email == email){
                return user;
            }
        }
        return User();
    }

    User getUserById(int id) {
        for(auto &user : all()) {
            if(user.id == id) return user;
        }
        return User(); 
    }

private:
    
    string toLower(string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }
};