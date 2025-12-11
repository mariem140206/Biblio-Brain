#pragma once
#include <string>
#include "User.h"
#include "Response.hpp"
#include "AuthService.h"
#include "crow.h"

using namespace std;

class UserController {
public:
    crow::response index(const crow::request& req);
    crow::response updateRole(const crow::request& req, int id);
    crow::response destroy(const crow::request& req, int id);
};
