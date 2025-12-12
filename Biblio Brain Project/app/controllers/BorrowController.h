#pragma once
#include "crow.h"
#include "../../core/Response.hpp"
#include "../models/Borrow.h"
#include "../models/Book.h"
#include "../models/User.h"
#include "../services/AuthService.h"
#include "../services/DateService.h"

class BorrowController {
public:
    static User getAuthenticatedUser(const crow::request& req);
    static crow::response requestBorrow(const crow::request& req);
    static crow::response processBorrowRequest(const crow::request& req, int id);
    static crow::response returnBook(const crow::request& req, int id);
    static crow::response myBorrows(const crow::request& req);
    static crow::response pendingRequests(const crow::request& req);
    static crow::response allBorrows(const crow::request& req);
};