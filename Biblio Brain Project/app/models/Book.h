#pragma once
#include <string>
#include <vector>
#include "../../core/BaseModel.hpp"

using namespace std;
using nlohmann::json;

/* ----------- Book Entity ----------- */
struct Book {
    int id = 0;
    string title;
    string author;
    string description;
    string category;
    int numberOfPages = 0;
    int year = 0;
    int totalCopies = 0;
    int availableCopies = 0;
    string imagePath;

    Book();

    json toJson() const;
    static Book fromJson(const json& j);

    bool isValid() const;
    bool isAvailable() const;
};

/* ----------- Book Model ----------- */
class BookModel : public BaseModel<Book> {
public:
    BookModel();

    void create(const Book& book);
    vector<Book> search(const string& query);
    bool decreaseAvailability(int bookId);
    bool increaseAvailability(int bookId);

private:
    string toLower(string s);
};

