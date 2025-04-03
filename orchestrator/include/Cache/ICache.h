#pragma once

template <typename Key, typename Value>
struct ICache {
    virtual ~ICache() = default;
    virtual void insert(const Key& key, const Value& value) = 0;
    virtual Value get(const Key &key) = 0;
    virtual bool exists(const Key &key) = 0;
};