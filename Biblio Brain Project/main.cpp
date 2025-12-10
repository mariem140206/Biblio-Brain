
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
([](const crow::request &req) {
    return BookController::search(req);
});
