
#pragma once
#include "../../core/BaseModel.hpp"
#include <string>
using namespace std;
struct Book {
    int id;
   string title;
    string author;
    string category;
    string description;
    string imagepath;
    int year;
    int availableCopies;


    Book();
    static Book fromJson(const json& j);
    json toJson() const;
    bool isValid() const;
    bool isAvailable() const;
};

class BookModel : public BaseModel<Book> {
public:
    BookModel();
    void create(const Book& book);
    bool decreaseAvailability(int bookId);
    bool increaseAvailability(int bookId);
};
