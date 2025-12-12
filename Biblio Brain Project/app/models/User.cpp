#include "../../core/BaseModel.hpp"
#include "User.h"
#include <string>
#include <algorithm>

 
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

    json User::toJson() const {
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

    
    void UserModel::create( User &user) {
        user.id = this->generateId();
        json data = this->getAllJson();
        data.push_back(user.toJson());
        this->saveJson(data);
    }

    vector<User> UserModel::search(const string &query) {
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


   User UserModel::findByEmail(const string& email){
        for (const auto& user : all()){
            if (user.email == email){
                return user;
            }
        }
        return User();
    }

    User UserModel::getUserById(int id) {
        for(auto &user : this->all()) {
            if(user.id == id) return user;
        }
        return User(); 
    }

    
    string UserModel::toLower(string s) {
        transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return tolower(c); });
        return s;
    }