#pragma once
#include "../../core/BaseModel.hpp"
#include <string>
#include <vector>

struct Borrow {
    int id;
    int userId;          
    int bookId;          
    string borrowDate;   
    string dueDate;      
    string returnDate;   
    string status;       

    Borrow() : id(-1), userId(-1), bookId(-1), borrowDate(""), dueDate(""), returnDate(""), status("active") {}

    static Borrow fromJson(const json &j) {
        Borrow b;
        if(j.contains("id")) b.id = j["id"].get<int>();
        if(j.contains("user_id")) b.userId = j["user_id"].get<int>();
        if(j.contains("book_id")) b.bookId = j["book_id"].get<int>();
        if(j.contains("borrow_date")) b.borrowDate = j["borrow_date"].get<string>();
        if(j.contains("due_date")) b.dueDate = j["due_date"].get<string>();
        if(j.contains("return_date")) b.returnDate = j["return_date"].get<string>();
        if(j.contains("status")) b.status = j["status"].get<string>();
        return b;
    }

    json toJson() const {
        return {
            {"id", id},
            {"user_id", userId},
            {"book_id", bookId},
            {"borrow_date", borrowDate},
            {"due_date", dueDate},
            {"return_date", returnDate},
            {"status", status}
        };
    }

    bool isValid() const { return id != -1; }
};

class BorrowModel : public BaseModel<Borrow> {
public:
    
    BorrowModel() : BaseModel("storage/borrows.json") {}

    void create(const Borrow &borrow) {
        json data = getAllJson();
        data.push_back(borrow.toJson());
        data.back()["id"] = generateId(); 
        saveJson(data);
    }

    bool returnBook(int borrowId, string actualReturnDate) {
        json data = getAllJson();
        bool found = false;

        for (auto &item : data) {
            if (item["id"] == borrowId) {
                item["status"] = "returned";          
                item["return_date"] = actualReturnDate; 
                found = true;
                break;
            }
        }

        if(found) {
            saveJson(data);
            return true;
        }
        return false;
    }

    vector<Borrow> getBorrowsByUserId(int userId) {
        vector<Borrow> userHistory;
        for(auto &b : all()) {
            if(b.userId == userId) {
                userHistory.push_back(b);
            }
        }
        return userHistory;
    }

    vector<Borrow> getActiveBorrows() {
        vector<Borrow> activeList;
        for(auto &b : all()) {
            if(b.status == "active") {
                activeList.push_back(b);
            }
        }
        return activeList;
    }
};