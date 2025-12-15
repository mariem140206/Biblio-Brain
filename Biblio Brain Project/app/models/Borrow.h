#pragma once
#include "../../core/BaseModel.hpp"
#include <string>
#include <vector>

using namespace std;
using nlohmann::json;

struct Borrow {
    int id;
    int userId;
    int bookId;
    string borrowDate;
    string dueDate;
    string returnDate;
    string status;
    bool returned = false;
    int duration = 15;

    Borrow();
    json toJson() const;
    static Borrow fromJson(const json &j);
    bool isValid() const;
};

class BorrowModel : public BaseModel<Borrow> {
public:
    BorrowModel();

    void create(const Borrow &borrow);
    bool returnBook(int borrowId, string actualReturnDate);
    vector<Borrow> findByUserId(int userId);
    vector<Borrow> findPending();
    bool hasActiveBorrow(int userId, int bookId);
    vector<Borrow> getBorrowsByUserId(int userId);
    vector<Borrow> getActiveBorrows(int userId);
};