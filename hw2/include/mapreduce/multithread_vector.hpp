#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>

template<typename T>
class LockVector {
public:
    LockVector() : vec(), mut(), cond() {}
    ~LockVector() {}


    void push_back(T& in) {
        std::lock_guard<std::mutex> lock(mut);
        vec.push_back(std::move(in));
        cond.notify_one();
    }
    T& operator[](const int index)
    {
        return vec[index];
    }
    int size() {
        return vec.size();
    }

private:
    std::vector<T> vec;
    std::mutex mut;
    std::condition_variable cond;
};
