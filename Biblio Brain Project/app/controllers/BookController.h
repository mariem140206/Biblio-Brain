#pragma once
#include "crow.h"
#include "../../core/Response.hpp"
#include "../models/Book.h"
#include "../services/AuthService.h"

class BookController {
public:
    static crow::response index(const crow::request& req);
    static crow::response show(const crow::request& req, int id);
    static crow::response create(const crow::request& req);
    static crow::response update(const crow::request& req, int id);
    static crow::response destroy(const crow::request& req, int id);
    static crow::response search(const crow::request& req);
    static crow::response searchByTitle(const crow::request&, const std::string& title);
    static crow::response searchByCategory(const crow::request&, const std::string& category);
    
private:
    static User getAuthenticatedUser(const crow::request& req);
};
