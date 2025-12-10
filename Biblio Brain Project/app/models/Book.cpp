#include "Book.h"
Book::Book()
    : id(-1), title(""), author(""), description(""),
      category(""), numberOfPages(0), year(0),
      totalCopies(0), availableCopies(0),
      imagePath("") {}

json Book::toJson() const {
    return {
        {"id", id},
        {"title", title},
        {"author", author},
        {"description", description},
        {"category", category},
        {"number_of_pages", numberOfPages},
        {"year", year},
        {"total_copies", totalCopies},
        {"available_copies", availableCopies},
        {"imagePath", imagePath}
    };
}

Book Book::fromJson(const json &j) {
    Book b;
    b.id = j.value("id", -1);
    b.title = j.value("title", "");
    b.author = j.value("author", "");
    b.description = j.value("description", "");
    b.category = j.value("category", "");
    b.numberOfPages = j.value("number_of_pages", 0);
    b.year = j.value("year", 0);
    b.totalCopies = j.value("total_copies", 0);
    b.availableCopies = j.value("available_copies", 0);
    b.imagePath = j.value("imagePath", "");
    return b;
}

bool Book::isValid() const { return id != -1; }
bool Book::isAvailable() const { return availableCopies > 0; }

BookModel::BookModel() : BaseModel("storage/books.json") {}

void BookModel::create(const Book &book) {
    json data = getAllJson();
    data.push_back(book.toJson());
    data.back()["id"] = generateId();
    saveJson(data);
}
string BookModel::toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

vector<Book> BookModel::search(const string &query) {
    vector<Book> results;
    string lowerQuery = toLower(query);

    for (auto &book : all()) {
        if (toLower(book.title).find(lowerQuery) != string::npos ||
            toLower(book.author).find(lowerQuery) != string::npos ||
            toLower(book.category).find(lowerQuery) != string::npos)
        {
            results.push_back(book);
        }
    }
    return results;
}

bool BookModel::decreaseAvailability(int bookId) {
    json data = getAllJson();
    for (auto &item : data) {
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
    for (auto &item : data) {
        if (item["id"] == bookId) {
            int available = item["available_copies"].get<int>();
            int total = item["total_copies"].get<int>();
            if (available < total) {
                item["available_copies"] = available + 1;
                saveJson(data);
                return true;
            }
        }
    }
    return false;
}
