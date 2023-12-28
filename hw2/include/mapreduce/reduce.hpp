#pragma once

#include "storage.hpp"
#include <thread>
#include <filesystem>
#include <iostream>
#include <filesystem>

const std::string OUT_DIR_PARENT = "../../data/"; 
const std::string OUT_DIR = "../../data/output/";
const std::string OUT_FILE = "out_"; // out_0.txt out_1.txt ...

std::string GetReduceFileName(const std::string& type, uint64_t file_num) {
    return OUT_DIR + type + std::to_string(file_num) + ".txt";
}

class Reduce{
public:
    void reduce(Storage& store, int id) {
        std::filesystem::create_directories(OUT_DIR); // Создаём дерево каталогов для верности
        const std::string file_name = GetReduceFileName(OUT_FILE, id);
        std::ofstream output(file_name.c_str());
        for (int i = 0; i < store.get_amount(); ++i) {
            auto res = store.get_ith(i);
            if (res) {
                auto taken = std::move(*res);
                
                size_t count = 0;
                for (auto val : taken.second) {
                    count += val;
                }
                output << taken.first << "\t" << count << std::endl;
            }
        }
        output.close();
    }
};
