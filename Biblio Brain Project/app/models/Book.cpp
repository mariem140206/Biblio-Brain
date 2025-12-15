#include "Book.h"

using namespace std;


#include "Book.h"

/* -------- Constructor -------- */
Book::Book() {
    id = 0;
    numberOfPages = 0;
    year = 0;
    totalCopies = 0;
    availableCopies = 0;
}

/* -------- Validation -------- */
bool Book::isValid() const {
    return id > 0 && !title.empty();
}

bool Book::isAvailable() const {
    return availableCopies > 0;
}

/* -------- JSON Serialization -------- */
json Book::toJson() const {
    return json{
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

Book Book::fromJson(const json& j) {
    Book b;
    if (j.contains("id")) b.id = j["id"];
    if (j.contains("title")) b.title = j["title"];
    if (j.contains("author")) b.author = j["author"];
    if (j.contains("description")) b.description = j["description"];
    if (j.contains("category")) b.category = j["category"];
    if (j.contains("number_of_pages")) b.numberOfPages = j["number_of_pages"];
    if (j.contains("year")) b.year = j["year"];
    if (j.contains("total_copies")) b.totalCopies = j["total_copies"];
    if (j.contains("available_copies")) b.availableCopies = j["available_copies"];
    if (j.contains("imagePath")) b.imagePath = j["imagePath"];
    return b;
}


BookModel::BookModel()
    : BaseModel("storage/books.json") {}

/* ---------------- CREATE ---------------- */
void BookModel::create(const Book& book) {
    json data = getAllJson();   // من BaseModel
    json newBook = book.toJson();
    newBook["id"] = generateId(); // من BaseModel
    data.push_back(newBook);
    saveJson(data);
}

/* ---------------- SEARCH ---------------- */
string BookModel::toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

vector<Book> BookModel::search(const string& query) {
    vector<Book> results;
    string q = toLower(query);

    for (auto& book : all()) {   // all() من BaseModel
        if (toLower(book.title).find(q) != string::npos ||
            toLower(book.author).find(q) != string::npos ||
            toLower(book.category).find(q) != string::npos)
        {
            results.push_back(book);
        }
    }
    return results;
}

/* ---------------- AVAILABILITY ---------------- */
bool BookModel::decreaseAvailability(int bookId) {
    json data = getAllJson();
    for (auto& item : data) {
        if (item["id"] == bookId && item["available_copies"] > 0) {
            item["available_copies"] =
                item["available_copies"].get<int>() - 1;
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
            int available = item["available_copies"];
            int total = item["total_copies"];
            if (available < total) {
                item["available_copies"] = available + 1;
                saveJson(data);
                return true;
            }
        }
    }
    return false;
}



