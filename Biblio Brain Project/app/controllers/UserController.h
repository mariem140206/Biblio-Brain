#pragma once
#include <string>
#include "../models/User.h"
#include "../../core/Response.hpp"
#include "../services/AuthService.h"
#include "crow.h"

using namespace std;
class UserController {
public:
  static crow::response index(const crow::request& req);
  static  crow::response updateRole(const crow::request& req, int id);
  static  crow::response destroy(const crow::request& req, int id);
};
