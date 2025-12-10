#include "AuthService.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include "../models/User.h" 
using namespace std;

unordered_map<string, Session> AuthService::sessions_by_refresh;
unordered_map<string, string> AuthService::access_to_refresh_map;
unordered_map<string, AccountLockState> AuthService::failed_attempts;
 const int AuthService:: MAX_ATTEMPTS = 3;
 const int AuthService:: LOCKOUT_DURATION = 60 * 3;

LoginResponse AuthService::login(const string& email, const string& password) {
    if (isAccountLocked(email)) return{"",""} ;
    UserModel model;
    User user = model.findByEmail(email);

    if (!user.isValid()){
     handleFailedLogin(email);
     return {"",""};
    }
   /* if (password != user.password || !PasswordHasher::verify(password, user.password))*/
    if (!PasswordHasher::verify(password, user.password)){
    handleFailedLogin(email);
     return {"",""};
    }
    if (failed_attempts.count(email)){
        failed_attempts.erase(email);
    }

    string accessToken = generateToken();
    string refreshToken = generateToken();
    time_t accessExpiry = time(0) + 20*60;
    time_t refreshExpiry = time(0) + 30*24*60*60;
    Session session {
        user,
        accessToken,
        refreshToken,
        accessExpiry,
        refreshExpiry,
    };
    sessions_by_refresh[refreshToken] = session;
    access_to_refresh_map[accessToken] = refreshToken;
    return  {accessToken, refreshToken};
}


bool AuthService::isAccountLocked(const string& email){
    if (failed_attempts.count(email)){
        AccountLockState& state = failed_attempts.at(email);
        if(time(0) < state.lockUntilTime){
            return true;
        }
        else{
            state.failedAttempts = 0;
            state.lockUntilTime = 0;
            return false;
        }
    }
    return false;
}


void AuthService::handleFailedLogin(const string& email){
    AccountLockState& state = failed_attempts[email];
    state.failedAttempts++;
    state.lastAttemptTime = time(0);
    if(state.failedAttempts >= MAX_ATTEMPTS){
        state.lockUntilTime = time(0) + LOCKOUT_DURATION;
    }
}


User AuthService::authenticate(const string& token) {
   if(access_to_refresh_map.count(token)){
    string refreshToken = access_to_refresh_map.at(token);
    Session& session = sessions_by_refresh.at(refreshToken);
    if(time(0) < session.accessExpiresAt){
        return session.user;
    }
    else{
        return User();
    }
   }
    return User();
}


void AuthService::logout(const string& token) {
    if(sessions_by_refresh.count(token)){
        string accessToken = sessions_by_refresh.at(token).accessToken;
        sessions_by_refresh.erase(token);
        access_to_refresh_map.erase(accessToken);
    }
}



string AuthService::refresh(const string& refreshToken){
    if(sessions_by_refresh.count(refreshToken)){
        Session& session = sessions_by_refresh.at(refreshToken);
        if (time(0) < session.refreshExpiresAt){
            access_to_refresh_map.erase(session.accessToken);
            string newAccessToken =generateToken();
            session.accessToken = newAccessToken;
            session.accessExpiresAt = time(0) + 20*60;
            access_to_refresh_map[newAccessToken] = refreshToken;
            return newAccessToken;
        }
        else{
            access_to_refresh_map.erase(session.accessToken);
            sessions_by_refresh.erase(refreshToken);
        }
    }
    return "";
}



bool AuthService::isAdmin(const User& user) { return user.role=="admin"; }
bool AuthService::isLibrarian(const User& user) { return user.role=="librarian" || user.role=="admin"; }


string AuthService::generateToken() { 
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, sizeof(alphanum)-2);

   string token ;
   for(int i=0;i<64;i++) token += alphanum[dis(gen)];
    return token;
}