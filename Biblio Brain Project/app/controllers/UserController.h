<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> c9808b8 (Add UserController files)
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
