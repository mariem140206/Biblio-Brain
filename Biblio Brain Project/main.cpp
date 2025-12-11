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
([](const crow::request &req) {
    return BookController::search(req);
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

