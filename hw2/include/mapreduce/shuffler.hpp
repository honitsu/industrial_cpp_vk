#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "storage.hpp"
#include <thread>
#include "multithread_vector.hpp"


class Shuffler
{
private:
public:
    void shuffle(Storage& store, LockVector<Storage>& output, int batch_size, int id) {
        Storage shuffled;
        //size_t id = std::hash<std::thread::id>{}(std::this_thread::get_id());
        //std::cout  << "Shuffler Run thread id " << id  << std::endl;
        for (int i = id; i < store.get_amount(); i += batch_size)
        {
            auto res = store.get_ith(i);
            if (res) {
                auto taken = std::move(*res);

                shuffled.pushback(taken.first , taken.second );
            
            }
            
        }
        output.push_back(shuffled);
    }
};
