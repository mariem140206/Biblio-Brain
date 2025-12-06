#pragma once
#include <string>
#include "crow.h"
#include "View.hpp"
#include "Response.hpp"
class BaseController {
protected:
    crow::SimpleApp *app;
public:
    BaseController(crow::SimpleApp *application) : app(application) {}
    std::string view(const std::string &templatePath,
    const crow::json::wvalue &data = {}) {
        return View::render(templatePath, data);
    }
    crow::response json(const crow::json::wvalue &data, int code = 200) {
        return Response::json(data, code);
    }
    crow::response success(const std::string &message,
    const crow::json::wvalue &data = {}) {
        return Response::success(message, data);
    }
    crow::response error(const std::string &message, int code = 400) {
        return Response::error(message, code);
    }
};
