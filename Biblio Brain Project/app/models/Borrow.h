#pragma once
#include "../../core/BaseModel.hpp"
#include <string>
#include <vector>

using namespace std;

struct Borrow {
    int id;
    int userId;
    int bookId;
    string borrowDate;
    string dueDate;
    string returnDate;
    string status;

    Borrow();

    static Borrow fromJson(const json &j);
    json toJson() const;
    bool isValid() const;
};

class BorrowModel : public BaseModel<Borrow> {
public:
    BorrowModel();

    void create(const Borrow &borrow);
    bool returnBook(int borrowId, string actualReturnDate);
    vector<Borrow> getBorrowsByUserId(int userId);
    vector<Borrow> getActiveBorrows();
};