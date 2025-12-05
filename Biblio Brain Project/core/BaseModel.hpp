#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "../utils/json.hpp"
#include "../app/models/JsonStorage.cpp"
using namespace std;
using json = nlohmann::json;
template<typename T>
class BaseModel {
    protected:
    string storagePath;
    public:
    BaseModel(const string &path):storagePath(path){}


    json getAllJson(){
        return JsonStorage::readFile(storagePath);
    }


    void saveJson(const json &data){
        JsonStorage::writeFile(storagePath,data);
    }


    int generateId(){
        json data = getAllJson();
        if(data.empty())
        return 1;
        int maxId = 0;
        for(auto &item : data){
            if(item.contains("id") && item["id"].is_number()){
                maxId = max(maxId, item["id"].get<int>());
            }
        }
        return maxId + 1;
    }


    vector<T> all(){
        vector<T> result;
        for(auto &item : getAllJson()){
            result.push_back(T::fromJson(item));
        }
        return result;
    }


    T findById(int id){
        for(auto &item : getAllJson()){
            if(item["id"] == id){
                return T::fromJson(item);
            }
        }
        return T();
    }


    bool deleteById(int id){
        json data = getAllJson();
        auto it = remove_if(data.begin(),data.end(),[id](const json &item)
    {return item["id"] == id});
    if(it != data.end()){
        data.erase(it,data.end());
        saveJson(data);
        return true;
    }
    return false;
    }


    bool update(int id, const json &upates){
        json data = getAllJson();
        for(auto &item : data){
            if(item["id"] == id){
                for(auto &[key,value] : updates.items()){
                    item[key] = value;
                }
                saveJson(data);
                return true;
            }
        }
        return false;
    }
};