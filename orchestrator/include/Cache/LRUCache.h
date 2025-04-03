#pragma once

#include "ICache.h"
#include <list>
#include <unordered_map>

template <typename Key, typename Value>
class LRUCache: public ICache<Key, Value> {
public:
    explicit LRUCache(const size_t capacity): capacity(capacity) {};

    void insert(const Key& key, const Value& value) override {
        if (exists(key)) {
            typename std::list<Key>::iterator it = keyToPosition[key];
            keyList.erase(it);
            keyToPosition.erase(key);
        }
        if (keyList.size() + 1 > capacity) {
            evict();
        }
        keyList.push_front(key);
        keyToPosition[key] = keyList.begin();
        entries[key] = value;
        std::cout << "INSERT: ";
        for (auto k: keyList)
            std::cout << k << ' ';
        std::cout << '\n';
    }

    Value get(const Key &key) override {
        if (!exists(key))
            throw std::runtime_error("Key not found");
        typename std::list<Key>::iterator it = keyToPosition[key];
        keyList.erase(it);
        keyList.push_front(key);
        keyToPosition[key] = keyList.begin();
        std::cout << "GET: ";
        for (auto k: keyList)
            std::cout << k << ' ';
        std::cout << '\n';
        return entries[key];
    }

    bool exists(const Key &key) override {
        return entries.find(key) != entries.end();
    }
private:
    const size_t capacity;
    std::list<Key> keyList;
    std::unordered_map<Key, Value> entries;
    std::unordered_map<Key, typename std::list<Key>::iterator> keyToPosition;

    void evict() {
        const Key& removable = keyList.back();
        entries.erase(removable);
        keyToPosition.erase(removable);
        keyList.pop_back();
    }
};

