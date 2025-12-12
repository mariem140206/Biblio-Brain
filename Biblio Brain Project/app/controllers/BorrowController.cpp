#include "BorrowController.h"
#include "../models/Borrow.h"
#include "../services/AuthService.h"
#include "../services/DateService.h"
#include "../../core/Response.hpp"
using namespace std;


User BorrowController::getAuthenticatedUser(const crow::request& req) {
    string token = req.get_header_value("Authorization");
    if (token.size() > 7 && token.substr(0, 7) == "Bearer ")
        token = token.substr(7);

    return AuthService::authenticate(token);
}


crow::response BorrowController::requestBorrow(const crow::request& req) {
    string token = req.get_header_value("Authorization");
    User user = AuthService::authenticate(token);
    if (!user.isValid()) return Response::unauthorized();

    auto body = crow::json::load(req.body);
    if (!body) return Response::error("Invalid JSON", 400);

    int bookId = body["book_id"].i();
    int duration = body.has("duration") ? body["duration"].i() : 14;

    BookModel bookModel;
    Book book = bookModel.findById(bookId);
    if (!book.isValid()) return Response::error("Book not found", 404);

    BorrowModel borrowModel;
    if (borrowModel.hasActiveBorrow(user.id, bookId))
        return Response::error("You already have an active request/borrow for this book", 400);

    Borrow borrow;
    borrow.userId = user.id;
    borrow.bookId = bookId;
    borrow.borrowDate = DateService::getCurrentDate();
    borrow.dueDate = DateService::addDays(borrow.borrowDate, duration);
    borrow.duration = duration;
    borrow.status = "pending";

    borrowModel.create(borrow);

    return Response::success("Borrow request submitted successfully");
}

crow::response BorrowController::processBorrowRequest(const crow::request& req, int id) {
    std::string token = req.get_header_value("Authorization");
    User user = AuthService::authenticate(token);
    if (!user.isValid()) return Response::unauthorized();
    if (!AuthService::isLibrarian(user)) return Response::forbidden("Only librarians can process borrow requests");

    auto body = crow::json::load(req.body);
    if (!body) return Response::error("Invalid JSON", 400);

    std::string action = body["action"].s();

    BorrowModel borrowModel;
    Borrow borrow = borrowModel.findById(id);
    if (!borrow.isValid()) return Response::error("Borrow request not found", 404);
    if (borrow.status != "pending") return Response::error("This request has already been processed", 400);

    if (action == "approve") {
        BookModel bookModel;
        Book book = bookModel.findById(borrow.bookId);
        if (!book.isAvailable()) return Response::error("Book is not available", 400);

        json updates;
        updates["status"] = "borrowed";
        updates["borrow_date"] = DateService::getCurrentDate();
        updates["due_date"] = DateService::addDays(updates["borrow_date"], borrow.duration);

        borrowModel.update(id, updates);
        bookModel.decreaseAvailability(borrow.bookId);

        return Response::success("Borrow request approved");
    } else if (action == "reject") {
        json updates;
        updates["status"] = "rejected";
        borrowModel.update(id, updates);

        return Response::success("Borrow request rejected");
    }

    return Response::error("Invalid action", 400);
}

crow::response BorrowController::returnBook(const crow::request& req, int id) {
    std::string token = req.get_header_value("Authorization");
    User user = AuthService::authenticate(token);
    if (!user.isValid()) return Response::unauthorized();

    BorrowModel borrowModel;
    Borrow borrow = borrowModel.findById(id);
    if (!borrow.isValid()) return Response::error("Borrow record not found", 404);

    if (borrow.userId != user.id && !AuthService::isLibrarian(user))
        return Response::forbidden("You cannot return this book");

    if (borrow.status != "borrowed") return Response::error("Book is not currently borrowed", 400);

    json updates;
    updates["status"] = "returned";
    updates["return_date"] = DateService::getCurrentDate();
    borrowModel.update(id, updates);

    BookModel bookModel;
    bookModel.increaseAvailability(borrow.bookId);

    return Response::success("Book returned successfully");
}

crow::response BorrowController::myBorrows(const crow::request& req) {
    std::string token = req.get_header_value("Authorization");
    User user = AuthService::authenticate(token);
    if (!user.isValid()) return Response::unauthorized();

    BorrowModel borrowModel;
    auto borrows = borrowModel.findByUserId(user.id);

    crow::json::wvalue::list borrowList;
    for (auto& borrow : borrows) {
        crow::json::wvalue item;
        item["id"] = borrow.id;
        item["book_id"] = borrow.bookId;
        item["borrow_date"] = borrow.borrowDate;
        item["due_date"] = borrow.dueDate;
        item["return_date"] = borrow.returnDate;
        item["status"] = borrow.status;
        item["duration"] = borrow.duration;
        borrowList.push_back(std::move(item));
    }

    crow::json::wvalue data;
    data = std::move(borrowList);

    return Response::success("Borrows retrieved", data);
}

crow::response BorrowController::pendingRequests(const crow::request& req) {
    std::string token = req.get_header_value("Authorization");
    User user = AuthService::authenticate(token);
    if (!user.isValid()) return Response::unauthorized();
    if (!AuthService::isLibrarian(user)) return Response::forbidden("Only librarians can view pending requests");

    BorrowModel borrowModel;
    auto borrows = borrowModel.findPending();

    BookModel bookModel;
    UserModel userModel;

    crow::json::wvalue::list borrowList;
    for (auto& borrow : borrows) {
        Book book = bookModel.findById(borrow.bookId);
        User borrowUser = userModel.findById(borrow.userId);

        crow::json::wvalue item;
        item["id"] = borrow.id;
        item["book_id"] = borrow.bookId;
        item["book_title"] = book.title;
        item["user_id"] = borrow.userId;
        item["user_name"] = borrowUser.name;
        item["user_email"] = borrowUser.email;
        item["borrow_date"] = borrow.borrowDate;
        item["due_date"] = borrow.dueDate;
        item["duration"] = borrow.duration;
        item["status"] = borrow.status;
        borrowList.push_back(std::move(item));
    }

    crow::json::wvalue data;
    data = std::move(borrowList);

    return Response::success("Pending requests retrieved", data);
}

crow::response BorrowController::allBorrows(const crow::request& req) {
    std::string token = req.get_header_value("Authorization");
    User user = AuthService::authenticate(token);
    if (!user.isValid()) return Response::unauthorized();
    if (!AuthService::isLibrarian(user)) return Response::forbidden("Only librarians can view all borrows");

    BorrowModel borrowModel;
    auto borrows = borrowModel.all();

    BookModel bookModel;
    UserModel userModel;

    crow::json::wvalue::list borrowList;
    for (auto& borrow : borrows) {
        Book book = bookModel.findById(borrow.bookId);
        User borrowUser = userModel.findById(borrow.userId);

        crow::json::wvalue item;
        item["id"] = borrow.id;
        item["book_id"] = borrow.bookId;
        item["book_title"] = book.title;
        item["user_id"] = borrow.userId;
        item["user_name"] = borrowUser.name;
        item["borrow_date"] = borrow.borrowDate;
        item["due_date"] = borrow.dueDate;
        item["return_date"] = borrow.returnDate;
        item["status"] = borrow.status;
        borrowList.push_back(std::move(item));
    }

    crow::json::wvalue data;
    data = std::move(borrowList);

    return Response::success("All borrows retrieved", data);
}
