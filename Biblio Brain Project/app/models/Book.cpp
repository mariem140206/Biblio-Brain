#include "Book.h"

Book::Book()
    : id(-1), title(""), author(""), year(0),description(""),
availableCopies(0), category("") , imagepath(""){}

Book Book::fromJson(const json& j) {
    Book b;
    b.id = j["id"];
    b.title = j["title"];
    b.author = j["author"];
    b.description=j["description"];
    b.year = j["year"];
    b.imagepath=j["image_path"];
    b.availableCopies = j["available_copies"];
    b.category = j["category"];
    return b;
}

json Book::toJson() const {
    return {
            {"id", id},
            {"title", title},
            {"author", author},
            {"year", year},
            {"available_copies", availableCopies},
            {"category", category},
            {"description",description},
            {"image_path",imagepath}
    };
}

bool Book::isValid() const { return id != -1; }
bool Book::isAvailable() const { return availableCopies > 0; }

BookModel::BookModel() : BaseModel("storage/books.json") {}

void BookModel::create(const Book& book) {
    json data = getAllJson();
    json bookJson = book.toJson();
    bookJson["id"] = generateId();
    data.push_back(bookJson);
    saveJson(data);
}

bool BookModel::decreaseAvailability(int bookId) {
    json data = getAllJson();
    for (auto& item : data) {
        if (item["id"] == bookId && item["available_copies"] > 0) {
            item["available_copies"] = item["available_copies"].get<int>() - 1;
            saveJson(data);
            return true;
        }
    }
    return false;
}

bool BookModel::increaseAvailability(int bookId) {
    json data = getAllJson();
    for (auto& item : data) {
        if (item["id"] == bookId) {
            int available = item["available_copies"].get<int>();
            item["available_copies"] = available + 1;
            saveJson(data);
            return true;
        }
    }
    return false;

}
