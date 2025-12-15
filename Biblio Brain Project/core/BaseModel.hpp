#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

class JsonStorage {
public:
    static json readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return json::array();
        json data;
        file >> data;
        return data;
    }
    
    static void writeFile(const std::string& path, const json& data) {
        std::ofstream file(path);
        file << data.dump(2);
    }
};

template<typename T>
class BaseModel {
protected:
    std::string storagePath;

public:
    BaseModel(const std::string& path) : storagePath(path) {}
    
    json getAllJson() {
        return JsonStorage::readFile(storagePath);
    }
    
    void saveJson(const json& data) {
        JsonStorage::writeFile(storagePath, data);
    }
    
    int generateId() {
        json data = getAllJson();
        if (data.empty()) return 1;
        int maxId = 0;
        for (auto& item : data) {
            if (item.contains("id") && item["id"].is_number()) {
                maxId = std::max(maxId, item["id"].get<int>());
            }
        }
        return maxId + 1;
    }
    
    std::vector<T> all() {
        std::vector<T> result;
        for (auto& item : getAllJson()) {
            result.push_back(T::fromJson(item));
        }
        return result;
    }
    
    T findById(int id) {
        for (auto& item : getAllJson()) {
            if (item["id"] == id) {
                return T::fromJson(item);
            }
        }
        return T();
    }
    
    bool deleteById(int id) {
        json data = getAllJson();
        auto it = std::remove_if(data.begin(), data.end(),
            [id](const json& item) { return item["id"] == id; });
        if (it != data.end()) {
            data.erase(it, data.end());
            saveJson(data);
            return true;
        }
        return false;
    }
    
    bool update(int id, const json& updates) {
        json data = getAllJson();
        for (auto& item : data) {
            if (item["id"] == id) {
                for (auto& [key, value] : updates.items()) {
                    item[key] = value;
                }
                saveJson(data);
                return true;
            }
        }
        return false;
    }
};