#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include "../../core/BaseModel.hpp"
using namespace std;
using nlohmann::json;
struct Book {
    int id;
    string title;
    string author;
    string description;
    string category;
    int numberOfPages;
    int year;
    int totalCopies;
    int availableCopies;
    string imagePath;

    Book();

    json toJson() const;
    static Book fromJson(const json &j);

    bool isValid() const;
    bool isAvailable() const;
};

class BookModel : public BaseModel<Book> {
public:
    BookModel();

    void create(const Book &book);
    vector<Book> search(const string &query);

    bool decreaseAvailability(int bookId);
    bool increaseAvailability(int bookId);

private:
    string toLower(string s);

};
