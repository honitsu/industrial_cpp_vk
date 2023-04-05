cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DDEBUG")
project(UString
	VERSION 1.0.0
	DESCRIPTION "Класс в стиле std::string для работы с utf-8"
	LANGUAGES CXX)
add_executable(catch2 testsuite/catch2.cpp ~/Catch2/extras/catch_amalgamated.cpp)
add_executable(showstats projects/showstats.cpp)
include_directories(include)
#include(CPack)
