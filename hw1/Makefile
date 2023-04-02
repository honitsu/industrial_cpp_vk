CC=g++
FLAGS=-std=c++20 -std=gnu++0x -Wall -pedantic -Werror -Wextra -DDEBUG
TEST_LIB=-lgtest_main -lgtest -lpthread

all: test runtest showstats cmake

test: tests.cpp ustring.hpp Makefile
	$(CC) $(FLAGS) tests.cpp -o test $(TEST_LIB)

showstats: showstats.cpp ustring.hpp Makefile
	$(CC) $(FLAGS) showstats.cpp -o showstats $(TEST_LIB)

test:
	./test 

runtest:
	./build/catch2 -s -i 

cmake:
	cmake -Bbuild -H. --fresh && (cd build; make)

clean:
	rm -f *.o test.exe test showstats showstats.exe
	rm -fr build
