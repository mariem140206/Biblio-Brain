#include "AuthController.h"
#include <string>
#include <iostream>
using namespace std;

crow::response AuthController::login(const crow::request &req)
{
    auto body = crow::json::load(req.body);
if (
    !body.count("email") || 
    !body.count("password") 
) {

    return Response::error(
        "Missing required fields", 400
    );
}

    string email = body["email"].s();
    string password = body["password"].s();


    LoginResponse response = AuthService::login(email, password);

    if (response.accessToken.empty())
    {
        return Response::error("Invalid credentials", 401);
    }

    crow::json::wvalue data;
    data["accessToken"] = response.accessToken;
    data["refreshToken"] = response.refreshToken;

    return Response::success("Login successful", data);
}

crow::response AuthController::registerUser(const crow::request& req)
{
    auto body = crow::json::load(req.body);
    if (!body) {
        return Response::error("Invalid JSON", 400);
    }

    if (!body.count("name") || !body.count("email") || !body.count("password")) {
        return Response::error(
            "Missing required fields: name, email, or password", 400
        );
    }

    User user;
    user.name  = body["name"].s();
    user.email = body["email"].s();
    user.password = body["password"].s(); 
    user.role = "user";

    std::string error;
    if (!AuthService::registerUser(user, error)) {
        return Response::error(error, 400);
    }

    return Response::success("Registration successful");
}


crow::response AuthController::logout(const crow::request &req)
{
    string token = req.get_header_value("Authorization");
    AuthService::logout(token);
    return Response::success("Logged out successfully");
}

crow::response AuthController::me(const crow::request &req)
{
    string token = req.get_header_value("Authorization");
    User user = AuthService::authenticate(token);

    if (!user.isValid())
    {
        return Response::unauthorized();
    }

    crow::json::wvalue data;
    data["id"] = user.id;
    data["name"] = user.name;
    data["email"] = user.email;
    data["role"] = user.role;
    data["phoneNumber"] = user.phoneNumber;
    data["dateOfBirth"] = user.dateOfBirth;

    return Response::success("User found", data);
}

crow::response AuthController::refresh(const crow::request &req)
{
    auto body = crow::json::load(req.body);
    if (!body || !body.count("refreshToken"))
    {
        return Response::error("Missing refresh token in body", 400);
    }
    string refreshToken = body["refreshToken"].s();
    string newAccessToken = AuthService::refresh(refreshToken);
    if (newAccessToken.empty())
    {
        return Response::unauthorized(); // 401 Unauthorized
    }
    crow::json::wvalue data;
    data["accessToken"] = newAccessToken;
    return Response::success("Token refreshed successfully", data);
}