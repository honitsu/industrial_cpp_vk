cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -std=gnu++0x -Wall -pedantic -Werror -Wextra")
project(UString
	VERSION 1.0.0
	DESCRIPTION "Класс в стиле std::string для работы с utf-8"
	LANGUAGES CXX)
add_executable(catch2 catch2.cpp ~/Catch2/extras/catch_amalgamated.cpp)
#include(CPack)
