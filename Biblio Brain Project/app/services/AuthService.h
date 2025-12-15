#pragma once
#include <string>
#include "PasswordHasher.h"
#include "../models/User.h"
#include <unordered_map>
#include <ctime>
using namespace std;


struct AccountLockState {
    int failedAttempts = 0;
    time_t lastAttemptTime = 0;
    time_t lockUntilTime = 0;
};


struct Session {
    User user;
    string accessToken;
    string refreshToken;
    time_t accessExpiresAt;
    time_t refreshExpiresAt;
};


struct LoginResponse{
    string accessToken;
    string refreshToken;
};


class AuthService {
public:
    static bool registerUser(const User& user, const std::string& roleValue, std::string& error);
    static LoginResponse login(const string& email, const string& password);
    static User authenticate(const string& token);
    static void logout(const string& accessToken);
    static bool isAdmin(const User& user);
    static bool isLibrarian(const User& user);
    static string refresh(const string& refreshToken);

private:
   static string generateToken();
   static unordered_map<string, Session> sessions_by_refresh;  
   static unordered_map<string,string> access_to_refresh_map; 
   static unordered_map<string, AccountLockState> failed_attempts;
   static constexpr int MAX_ATTEMPTS = 3;
   static constexpr int LOCKOUT_DURATION = 60 * 3;
   static void handleFailedLogin(const string& email);
   static bool isAccountLocked(const string& email);
};