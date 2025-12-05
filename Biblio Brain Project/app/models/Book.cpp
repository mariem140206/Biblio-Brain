#pragma once
#include "../../core/BaseModel.hpp"
#include <string>
using namespace std;

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

    Book() : id(-1), title(""), author(""), description("") , numberOfPages(0) , year(0), totalCopies(0), availableCopies(0), category("") {}
     static Book fromJson(const json &j) {

      Book b;
      b.id = j["id"].get<int>();
      b.title = j["title"].get<string>();
      b.author = j["author"].get<string>();
      b.description = j["description"].get<string>();
      b.category = j["category"].get<string>();
      b.year = j["year"].get<int>(); 
      b.totalCopies = j["total_copies"].get<int>(); 
      b.availableCopies = j["available_copies"].get<int>(); 
      return b;

}


 json toJson() const {
    return {
        {"id", id},
        {"title", title},
        {"author", author},
        {"description",description},
        {"category", category}
        {"year", year},
        {"total_copies", totalCopies},
        {"available_copies", availableCopies}
    };
 }

 bool isValid() const { return id !=-1; }
 bool isAvailable() const { return availableCopies > 0; }

};

 class BookModel : public BaseModel<Book> {
    public:
      BookModel() : BaseModel("storage/books.json"){}


      void create(const Book &book){
        json data = getAllJson();
        data.push_back(book.toJson());
        data.back()["id"] = generateId();
        saveJson(data);
      }


      vector<Book> search(const string &query){
        vector<Book> results;
        string lowerQuery = toLower(query);
        for (auto &book : all()){
            if (toLower(book.title).find(lowerQuery)) != string::npos||
            toLower(book.author).find(lowerQuery) != string::npos ||
             toLower(book.category).find(lowerQuery) != string::npos  {
             results.push_back(book);
        }

      }
      return results;
      }


      bool decreaseAvailability (int bookId){
        json data = getAllJson();
        for (auto &item : data){
            if (item["id"] == bookId && item["available_copies"] > 0){
                item["available_copies"] = item["available_copies"].get<int>()- 1;
                saveJson(data);
                return true;
            }
        }
        return false;
      }


      bool increaseAvailability(int bookId){
        json data = getAllJson();
        for (auto &item : data){
            if(item["id"] == bookId){
                int available = item["available_copies"].get<int>();
                int total = item["total_copies"].get<int>();
                if(available < total) {
                    item["available_copies"] = available + 1;
                    saveJson(data);
                    return true;
                }
            }
        }
        return false;
      }


    private :
        string toLower(string s ){
            std::transform(s.begin(), s.end(), s.begin(), ::tolower);
            return s;
        }
   
};