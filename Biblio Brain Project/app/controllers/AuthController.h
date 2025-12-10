#pragma once
#include "crow.h"
#include "../../core/Response.hpp"
#include "../models/User.h"
#include "../services/AuthService.h"

class AuthController {
public:
    static crow::response login(const crow::request& req);
    static crow::response registerUser(const crow::request& req);
    static crow::response logout(const crow::request& req);
    static crow::response me(const crow::request& req);
    static crow::response refresh(const crow::request& req);
};