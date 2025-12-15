 #include "crow.h"

// Controllers
#include "app/controllers/AuthController.h"
#include "app/controllers/BookController.h"
#include "app/controllers/BorrowController.h"
#include "app/controllers/UserController.h"

#include <fstream>
#include <sstream>
#include <map>

int main()
{
    crow::App<> app;

    /* ============================================
       Serve Static Files Under  /assets/<path>
       ============================================ */
    CROW_ROUTE(app, "/assets/<path>")
    ([](std::string path) {
        std::string fullPath = "../public/" + path;
        std::ifstream file(fullPath, std::ios::binary);

        if (!file.is_open())
            return crow::response(404);

        // Read file
        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        crow::response res(content);

        // MIME types
        static std::map<std::string, std::string> mimeTypes = {
            {".css", "text/css"}, {".js", "application/javascript"},
            {".png", "image/png"}, {".jpg", "image/jpeg"},
            {".jpeg", "image/jpeg"}, {".gif", "image/gif"},
            {".svg", "image/svg+xml"}, {".woff", "font/woff"},
            {".woff2", "font/woff2"}, {".ttf", "font/ttf"},
            {".ico", "image/x-icon"}
        };

        // Detect content type
        for (auto &m : mimeTypes) {
            const auto &ext = m.first;
            const auto &type = m.second;

            if (path.size() >= ext.size() &&
                path.compare(path.size() - ext.size(), ext.size(), ext) == 0)
            {
                res.set_header("Content-Type", type);
                break;
            }
        }

        return res;
    });

    /* ============================================
       Serve Index Page
       ============================================ */
    CROW_ROUTE(app, "/")
    ([]() {
        crow::mustache::set_base("../public");
        auto page = crow::mustache::load("index.html");
        return page.render();
    }); 
  
  /* ============================================
       Auth Routes
       ============================================ */
    CROW_ROUTE(app, "/api/auth/login").methods("POST"_method)
    ([](const crow::request &req) { return AuthController::login(req); });

    CROW_ROUTE(app, "/api/auth/register").methods("POST"_method)
    ([](const crow::request &req) { return AuthController::registerUser(req); });

    CROW_ROUTE(app, "/api/auth/logout").methods("POST"_method)
    ([](const crow::request &req) { return AuthController::logout(req); });

    CROW_ROUTE(app, "/api/auth/me")
    ([](const crow::request &req) { return AuthController::me(req); });

     CROW_ROUTE(app, "/api/auth/refresh")
    ([](const crow::request &req) { return AuthController::refresh(req); });


/* Book Routes
   ============================================ */

CROW_ROUTE(app, "/api/books")
([](const crow::request &req) { return BookController::index(req); });

CROW_ROUTE(app, "/api/books/<int>")
([](const crow::request &req, int id) { return BookController::show(req, id); });

CROW_ROUTE(app, "/api/books").methods("POST"_method)
([](const crow::request &req) { return BookController::create(req); });

CROW_ROUTE(app, "/api/books/<int>").methods("PUT"_method)
([](const crow::request &req, int id) { return BookController::update(req, id); });

CROW_ROUTE(app, "/api/books/<int>").methods("DELETE"_method)
([](const crow::request &req, int id) { return BookController::destroy(req, id); });

CROW_ROUTE(app, "/api/books/search")
([](const crow::request& req) {
    auto title = req.url_params.get("title");
    auto category = req.url_params.get("category");

    if (title)
        return BookController::searchByTitle(req, title);

    if (category)
        return BookController::searchByCategory(req, category);

    return crow::response(400, "Missing search parameter");
});



    /* ============================================
       Borrow Routes
       ============================================ */
    CROW_ROUTE(app, "/api/borrows").methods("POST"_method)
    ([](const crow::request &req) { return BorrowController::requestBorrow(req); });

    CROW_ROUTE(app, "/api/borrows/<int>/process").methods("POST"_method)
    ([](const crow::request &req, int id) { return BorrowController::processBorrowRequest(req, id); });

    CROW_ROUTE(app, "/api/borrows/<int>/return").methods("POST"_method)
    ([](const crow::request &req, int id) { return BorrowController::returnBook(req, id); });

    CROW_ROUTE(app, "/api/borrows/my")
    ([](const crow::request &req) { return BorrowController::myBorrows(req); });

    CROW_ROUTE(app, "/api/borrows/pending")
    ([](const crow::request &req) { return BorrowController::pendingRequests(req); });

    CROW_ROUTE(app, "/api/borrows/all")
    ([](const crow::request &req) { return BorrowController::allBorrows(req); });

        /* ============================================
       User Routes
       ============================================ */
    CROW_ROUTE(app, "/api/users")
    ([](const crow::request &req) { return UserController::index(req); });

    CROW_ROUTE(app, "/api/users/<int>/role").methods("PUT"_method)
    ([](const crow::request &req, int id) { return UserController::updateRole(req, id); });

    CROW_ROUTE(app, "/api/users/<int>").methods("DELETE"_method)
    ([](const crow::request &req, int id) { return UserController::destroy(req, id); });

    /* ============================================
       Run Server
       ============================================ */
    app.port(8080).multithreaded().run();

    return 0;
}

