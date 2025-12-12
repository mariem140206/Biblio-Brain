#pragma once
#include "crow.h"
#include <string>
using namespace std;

class Response {
public:
    static crow::response json(const crow::json::wvalue& data, int code = 200) {
        crow::response res(code);
        res.add_header("Content-Type", "application/json");
        res.add_header("Access-Control-Allow-Origin", "*");
        res.body = data.dump();
        return res;
    }

    static crow::response success(const string& message, const crow::json::wvalue& data)
    {
        crow::json::wvalue res;
        res["success"] = true;
        res["message"] = message;
        res["data"] = data.dump();   
        return res;
    }

    static crow::response success(const string& message) {
        crow::json::wvalue res;
        res["success"] = true;
        res["message"] = message;
        return json(res);
    }

    static crow::response error(const string& message, int code = 400) {
        crow::json::wvalue res;
        res["success"] = false;
        res["error"] = message;
        return json(res, code);
    }

    static crow::response unauthorized(const string& message = "Unauthorized") {
        return error(message, 401);
    }

    static crow::response forbidden(const string& message = "Access denied") {
        return error(message, 403);
    }

    static crow::response notFound(const string& message = "Resource not found") {
        return error(message, 404);
    }
};
