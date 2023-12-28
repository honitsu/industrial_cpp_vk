#pragma once

#include <iterator>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <optional>

#include <map>
#include <vector>
#include <mutex>
#include <condition_variable>

class Storage 
{
public:
    using iterator_t = typename std::map<std::string, std::vector<int>>::iterator;
    using const_iterator_t = typename std::map<std::string, std::vector<int>>::const_iterator;

    Storage() : cond() {
        mut = new std::mutex();
        cond = new std::condition_variable();
    }
    ~Storage() {}

    void pushback(const std::string& key, const int& value)
    {
        std::lock_guard<std::mutex> lock(*mut);
        if (!store.count(key))
            store[key] = {};
        store[key].push_back(value);
        cond->notify_one();
    }

    void pushback(const std::string& key, const std::vector<int>& values)
    {
        std::lock_guard<std::mutex> lock(*mut);
        if (!store.count(key))
            store[key] = {};
        store[key].insert(store[key].end(), std::begin(values), std::end(values));
        cond->notify_one();
    }

    auto get_key_counts() const
    {
        std::map<std::string, std::size_t> counts;
        for (const auto& [key, lst] : store)
            counts[key] = lst.size();
        return counts;
    }

    auto get_keys() const
    {
        std::vector<std::string> keys;
        boost::copy(store | boost::adaptors::map_keys, std::back_inserter(keys));
        return keys;
    }

    bool is_key_present(const std::string& key) const { return store.count(key); }

    const auto& get_key_values(const std::string& key) const
    {
        return store.at(key);
    }

    size_t get_amount()
    {
        return store.size();
    }

    std::optional<std::pair<std::string, std::vector<int>>> get_ith(int i) {
        int count = 0;
        for (auto line : store) {
            if (count == i) {
                return std::optional<std::pair<std::string, std::vector<int>>>{line};
            }
            count ++;
        }
        return std::nullopt;
    }

    void print() const
    {
        std::cout << "Amount " << store.size() << std::endl;
        for (auto line : store)
        {
            std::cout << "Key is " << line.first << std::endl;
            std::vector<int> values = line.second;
            for (auto value : values)
            {
                std::cout << "Value is " << value << " ";
            }
            std::cout << "\n";
        }
        
    }

private:
    std::map<std::string, std::vector<int>> store;
    std::mutex* mut;
    std::condition_variable* cond;
};   
