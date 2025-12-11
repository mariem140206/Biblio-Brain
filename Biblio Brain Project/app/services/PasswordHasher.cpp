#include "PasswordHasher.h"
#include <string>
#include <stdexcept>
#include "external_secure_hashing_library/hasher.h"
using namespace std;

string PasswordHasher::hash(const string& password){
    if (password.empty()){
        return "";
    }
    try{
        return SecureHasher::create(password);
    }
    catch (const exception& e){
        cerr << "FATAL: Hashing failed: "<<e.what() << endl;
        return "";
    }
}
bool PasswordHasher::verify(const string& password, const string& hash) {
    if (password.empty() || hash.empty()) {
        return false;
    }
    try{
   return SecureHasher::verify(password, hash);
    }
    catch(const exception& e){
    cerr << "FATAL: Verification error: "<<e.what() << endl;
        return false;     
    }

