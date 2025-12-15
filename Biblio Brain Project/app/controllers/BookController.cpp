#include "BookController.h"
#include <utility>
using namespace std;


crow::json::wvalue toCrowJson(const Book& b) {
    crow::json::wvalue json;
    json["id"] = b.id;
    json["title"] = b.title;
    json["author"] = b.author;
    json["category"] = b.category;
    json["year"] = b.year;
    json["imagePath"] = b.imagePath;
    return json;
}


crow::response BookController::searchByTitle(const crow::request& req, const std::string& title) {
    auto books = JsonStorage::readFile("../storage/books.json"); 
    json result = json::array();

    for (auto& book : books) {
        if (book["title"].get<std::string>() == title) {
            result.push_back(book);
        }
    }

    if (result.empty()) return crow::response(404, "Book not found");
    return crow::response(result.dump());
}


crow::response BookController::searchByCategory(const crow::request& req, const std::string& category) {
    auto books = JsonStorage::readFile("../storage/books.json"); 
    json result = json::array();

    for (auto& book : books) {
        if (book["category"].get<std::string>() == category) {
            result.push_back(book);
        }
    }

    if (result.empty()) return crow::response(404, "No books found in this category");
    return crow::response(result.dump());
}


User BookController::getAuthenticatedUser(const crow::request& req) {
    string token = req.get_header_value("Authorization");
    if (token.rfind("Bearer ", 0) == 0)
        token = token.substr(7);

    return AuthService::authenticate(token);
}


crow::response BookController::index(const crow::request& req) {
    BookModel model;
    auto books = model.all();

    crow::json::wvalue::list bookList;
    for (auto& b : books)
        bookList.push_back(toCrowJson(b));

    crow::json::wvalue data;
    data["books"] = std::move(bookList);

    return Response::success("Books retrieved", data);
}


crow::response BookController::show(const crow::request& req, int id) {
    BookModel model;
    Book book = model.findById(id);
    if (!book.isValid())
        return Response::error("Book not found", 404);

    return Response::success("Book found", toCrowJson(book));
}


crow::response BookController::search(const crow::request& req) {
    auto query = req.url_params.get("q");
    if (!query)
        return Response::error("Missing search query", 400);

    BookModel model;
    auto results = model.search(query);

    crow::json::wvalue::list list;
    for (auto& book : results)
        list.push_back(toCrowJson(book));

    crow::json::wvalue data;
    data["results"] = std::move(list);

    return Response::success("Search completed", data);
}


crow::response BookController::create(const crow::request& req) {
    User user = getAuthenticatedUser(req);
    if (!user.isValid())
        return Response::unauthorized();
    if (!AuthService::isLibrarian(user))
        return Response::forbidden("Only librarians can add books");

    auto body = crow::json::load(req.body);
    if (!body)
        return Response::error("Invalid JSON", 400);

    Book book;
    book.title = body["title"].s();
    book.author = body["author"].s();
    book.description = body["description"].s();
    book.category = body["category"].s();
    book.numberOfPages = body["number_of_pages"].i();
    book.year = body["year"].i();
    book.totalCopies = body["total_copies"].i();
    book.availableCopies = body["available_copies"].i();
    if (body.has("imagePath")) book.imagePath = body["imagePath"].s();

    BookModel model;
    model.create(book);

    return Response::success("Book created successfully");
}


crow::response BookController::update(const crow::request& req, int id) {
    User user = getAuthenticatedUser(req);
    if (!user.isValid())
        return Response::unauthorized();
    if (!AuthService::isLibrarian(user))
        return Response::forbidden("Only librarians can update books");

    auto body = crow::json::load(req.body);
    if (!body)
        return Response::error("Invalid JSON", 400);

    BookModel model;
    Book book = model.findById(id);
    if (!book.isValid())
        return Response::error("Book not found", 404);

    if (body.has("title")) book.title = body["title"].s();
    if (body.has("author")) book.author = body["author"].s();
    if (body.has("description")) book.description = body["description"].s();
    if (body.has("category")) book.category = body["category"].s();
    if (body.has("number_of_pages")) book.numberOfPages = body["number_of_pages"].i();
    if (body.has("year")) book.year = body["year"].i();
    if (body.has("total_copies")) book.totalCopies = body["total_copies"].i();
    if (body.has("available_copies")) book.availableCopies = body["available_copies"].i();
    if (body.has("imagePath")) book.imagePath = body["imagePath"].s();

    model.update(id, book.toJson());

    return Response::success("Book updated successfully");
}

crow::response BookController::destroy(const crow::request& req, int id) {
    User user = getAuthenticatedUser(req);
    if (!user.isValid())
        return Response::unauthorized();
    if (!AuthService::isAdmin(user))
        return Response::forbidden("Only admins can delete books");

    BookModel model;
    if (model.deleteById(id))
        return Response::success("Book deleted successfully");

    return Response::error("Book not found", 404);
}


