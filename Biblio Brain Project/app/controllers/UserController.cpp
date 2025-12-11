#include "UserController.h"

crow::response UserController::index(const crow::request& req) {
    string token = req.get_header_value("Authorization");
    string token = extractToken(header);
    User user = AuthService::authenticate(token);
    if (!user.isValid()) return Response::unauthorized();
    if (!AuthService::isAdmin(user)) return Response::forbidden("Only admins can view all users");

    UserModel model;
    auto users = model.all();

    crow::json::wvalue::list userList;
    for (auto& u : users) {
        crow::json::wvalue item;
        item["id"] = u.id;
        item["name"] = u.name;
        item["email"] = u.email;
        item["role"] = u.role;
        userList.push_back(std::move(item));
    }

    crow::json::wvalue data;
    data = std::move(userList);

    return Response::success("Users retrieved", data);
}

crow::response UserController::updateRole(const crow::request& req, int id) {
    string token = req.get_header_value("Authorization");
    string token = extractToken(header);
    User user = AuthService::authenticate(token);
    if (!user.isValid()) return Response::unauthorized();
    if (!AuthService::isAdmin(user)) return Response::forbidden("Only admins can update user roles");

    auto body = crow::json::load(req.body);
    if (!body) return Response::error("Invalid JSON", 400);

    string newRole = body["role"].s();
    if (newRole != "admin" && newRole != "librarian" && newRole != "user")
        return Response::error("Invalid role", 400);

    UserModel model;
    json updates;
    updates["role"] = newRole;

    if (model.update(id, updates)) {
        return Response::success("User role updated successfully");
    }

    return Response::error("User not found", 404);
}

crow::response UserController::destroy(const crow::request& req, int id) {
    string token = req.get_header_value("Authorization");
    string token = extractToken(header);
    User user = AuthService::authenticate(token);
    if (!user.isValid()) return Response::unauthorized();
    if (!AuthService::isAdmin(user)) return Response::forbidden("Only admins can delete users");

    if (user.id == id) return Response::error("You cannot delete yourself", 400);

    UserModel model;
    if (model.deleteById(id)) {
        return Response::success("User deleted successfully");
    }

    return Response::error("User not found", 404);
}

namespace {
    string extractToken(const string& header) {
        if (header.size() > 7 && header.substr(0, 7) == "Bearer ") {
            return header.substr(7);
        }
        return header;
    }
}
