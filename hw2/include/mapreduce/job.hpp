#pragma once

#include "shuffler.hpp"

#include <chrono>
//#include <algorithm>
#include <map>
//#include <queue>
#include <vector>
#include <thread>
#include <atomic>
#include "map.hpp"
#include "storage.hpp"
#include "reduce.hpp"
#include <string>
#include <mutex>
#include <condition_variable>
#include "multithread_vector.hpp"

/*class Synchronization {
public:
    Synchronization(const std::size_t num_threads) : 
      _num_threads(num_threads) {
    }

    void synch() {
        std::unique_lock lock(m);
        ++thread_cnt;
        if (thread_cnt >= _num_threads) {
            cv.notify_all();
        } else {
            while (thread_cnt < _num_threads) {
                cv.wait(lock);
            }
        }
        ++thread_out;
        if (thread_out == _num_threads) {
            thread_cnt = 0;
            thread_out = 0;
        }
    }

private:
    std::atomic_int thread_out{0};
    std::atomic_int thread_cnt{0};
    const size_t _num_threads = 0;
    std::mutex m;
    std::condition_variable cv;
};*/

void run_map_phase(/*std::string& str, Synchronization& synchron, */Map& map_fn, Storage& istore, int id, LockVector<std::string>& files, int batch_size, int total)
{
    map_fn.map(/*str,*/ istore, id, files, batch_size, total);
    //synchron.synch();
}

void run_shuffler_phase(/*Synchronization& synchron,*/ Storage& istore, LockVector<Storage>& shuffled, int reducers_count, Shuffler& shuffler, int id)
{
    shuffler.shuffle(istore, shuffled, reducers_count, id);
    //synchron.synch();
}

void run_reducer_phase(Storage& shuffled, int id)
{
    Reduce fn;
    fn.reduce(shuffled, id);
}

class Job
{
public:

    Job(Map& map_fn, Storage& is, Shuffler& cfn, Reduce& reduce_fn, Storage& output_store)
        : map_fn(map_fn), shuffler(cfn), istore(is), reduce_fn(reduce_fn), output_store(output_store)
    {
    }

    ~Job()
    {
    }

    void start()
    {
       
        unsigned int n = std::thread::hardware_concurrency();
        
        //Synchronization synchronizatorMap(map_workers); //не нужен
        
        LockVector<std::string> files;   
        std::string s1 = "../../projects/src/dummyfiles/file1.txt";                                                        //заменить !
        std::string s2 = "../../projects/src/dummyfiles/file2.txt";    
        std::string s3 = "../../projects/src/dummyfiles/file3.txt";    
        std::string s4 = "../../projects/src/dummyfiles/file3.txt";    

        files.push_back(s1);
        files.push_back(s2);
        files.push_back(s3);
        files.push_back(s4);
        int batch_size = files.size();
        if (batch_size > n) {
            batch_size = n;
        }
        int map_workers = batch_size;

        std::vector<std::thread> map_threads = {};
        for (int i = 0; i < map_workers; ++i) {
            int thread_id = i;
            std::thread map_thread(run_map_phase, /*std::ref(files[i]), std::ref(synchronizatorMap),*/ std::ref(map_fn), std::ref(istore),  thread_id, std::ref(files), batch_size, files.size());
            map_threads.emplace_back(std::move(map_thread));
        }

        for (int i = 0; i < map_workers; ++i) {
            map_threads[i].join();
        }
        //Mapping completed
//        istore.print();
        size_t reducers_count = istore.get_amount();
       // unsigned int n = std::thread::hardware_concurrency();
        if (reducers_count > n) {
            reducers_count = n;
        }
        //Synchronization synchronizatorShuffler(reducers_count); // Не нужен
        LockVector<Storage> shuffled;
        std::vector<std::thread> shuffle_threads = {};
//        std::cout << "\n\n\n\n";
//        std::cout << "Shuffling " << reducers_count << " threads" << std::endl;
        for (size_t i = 0; i < reducers_count; ++i) {
            int thread_id = i;
            std::thread shuffle_thread(run_shuffler_phase, /*std::ref(synchronizatorShuffler), */std::ref(istore), std::ref(shuffled), std::ref(reducers_count), std::ref(shuffler), thread_id);
            shuffle_threads.emplace_back(std::move(shuffle_thread));
        }
        for (size_t i = 0; i < reducers_count; ++i) {
            shuffle_threads[i].join();
        }
/*
        std::cout << "\n\n\n\n";
        for (int i = 0; i < shuffled.size(); ++i) {
            std::cout << "New batch #" << i << std::endl;
            shuffled[i].print();
        }
*/
        //Shuffling completed!
        std::vector<std::thread> reducer_threads = {};

//        std::cout << "Reducing " << reducers_count << " threads" << std::endl;
        for (size_t i = 0; i < reducers_count; ++i) {
            int thread_id = i;
            std::thread reducer_thread(run_reducer_phase, std::ref(shuffled[i]), thread_id);
            reducer_threads.emplace_back(std::move(reducer_thread));
        }

        for (size_t i = 0; i < reducers_count; ++i) {
            reducer_threads[i].join();
        }
//        std::cout << "\n\n\n\n";

        
    }

private:
    
    Map& map_fn;
    Storage& istore; // accumulates intermediates from successive tasks
    Shuffler& shuffler;
    Reduce& reduce_fn;
    Storage& output_store;
};
