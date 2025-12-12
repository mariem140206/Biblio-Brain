<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> c9808b8 (Add UserController files)
#pragma once
#include <string>
#include "../models/User.h"
#include "../../core/Response.hpp"
#include "../services/AuthService.h"
#include "crow.h"

using namespace std;
string extractToken(const string& headerValue) ;
class UserController {
public:
  static crow::response index(const crow::request& req);
  static  crow::response updateRole(const crow::request& req, int id);
  static  crow::response destroy(const crow::request& req, int id);
};
<<<<<<< HEAD
=======
//
// Created by dell on 12/10/2025.
//

#ifndef BIBLIO_BRAIN_PROJECT_USERCONTROLLER_H
#define BIBLIO_BRAIN_PROJECT_USERCONTROLLER_H

#endif //BIBLIO_BRAIN_PROJECT_USERCONTROLLER_H
>>>>>>> 50a7dcf (Add UserController files)
=======
>>>>>>> c9808b8 (Add UserController files)
=======
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
>>>>>>> 1e864c1 (add storage)
