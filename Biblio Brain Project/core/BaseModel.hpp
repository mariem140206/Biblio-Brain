#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "../utils/json.hpp"
using namespace std;
using json = nlohmann::json;

template<typename T>
class BaseModel {
    protected:
    string storagePath;
    json cachedData;
    public:
    BaseModel(const string &path):storagePath(path){
        cachedData = json::array();
    }


    json getAllJson(){
        return cachedData;
       /* return JsonStorage::readFile(storagePath);*/
    }


    void saveJson(const json &data){
        cachedData = data;
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


     bool deleteById(int id){
        json data = getAllJson();
        auto it = remove_if(data.begin(),data.end(),[id](const json &item)
    {return item.contains("id") && item["id"].is_number() && item["id"].get<int>() == id;});
    if(it != data.end()){
        data.erase(it, data.end());
        saveJson(data);
        return true;
    }
    return false;
    }


    vector<T> all(){
        vector<T> result;
        json data = getAllJson();
        for(auto &item : data){
            result.push_back(T::fromJson(item));
        }
        return result;
    }


    T findById(int id){
        json data = getAllJson();
        for(auto &item : data){
            if(item.contains("id") && item["id"].is_number() && item["id"].get<int>() == id){
                return T::fromJson(item);
            }
        }
        return T();
    }


    bool update(int id, const json &updates){
        json data = getAllJson();
        for(auto &item : data){
            if(item.contains("id") && item["id"].is_number() && item["id"].get<int>() == id){
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