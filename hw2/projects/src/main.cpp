#include "job.hpp"
#include "map.hpp"
#include "shuffler.hpp"
#include "reduce.hpp"
#include "storage.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <numeric>


int main (int argc, char* argv[])
{
    std::string source_dir = "../../projects/src/dummyfiles";
    Map mapfn;
    Shuffler shuffler;
    Storage intermediate_store;
    Reduce reducefn;
    Storage output_store;

    Job job(mapfn, intermediate_store, shuffler, reducefn, output_store);

    
    
    job.start(); //запуск
    
    return 0;
}
