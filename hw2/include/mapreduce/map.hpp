#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <boost/range/istream_range.hpp>
#include "storage.hpp"
#include "multithread_vector.hpp"

class Map {
public:
    void map(/*std::string filename,*/ Storage& store, int id, LockVector<std::string>& filenames, int batch_size, int total) {
        for (int i = id; i < total; i += batch_size ) {
            std::ifstream ifs(filenames[i]);
            //std::cout << "Key " << filename << std::endl;
                                    
            for (const auto& word : boost::range::istream_range<std::string>(ifs))
            {
                //std::cout << "word " << word << std::endl;
                if (word.size() > 100)
                    continue;
                store.pushback(word, 1);
            }
        }
    }
};
