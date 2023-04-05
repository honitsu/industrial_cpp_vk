// showstats.cpp
//
// Программа считывает строку со стандартного ввода, 
// добавляет символ с помощью push_back() и удаляет его с pop_pack()
// после чего выводит в стандартный вывод значения, разделённые табуляцией:
// ---------------------------
// Число_байт
// Число символов UTF-8
// Число символов длиной 1 байт
// Число символов длиной 2 байта
// Число символов длиной 3 байта
// Число символов длиной 4 байта
// Исходная строка текста
//

#include "ustring.hpp"
#include <iostream>
#include <string>

int main()
{
	std::string line;
	UString u1;

	while(getline(std::cin, line)) 
	{
		u1 = line;
		u1.push_back(U'Ё');
		u1.pop_back();
		size_t cnt = 0;
		size_t tmp;
		std::cout << u1.size() << '\t' << u1.length() << '\t' << u1.count_by_size(1) << 
			'\t' << u1.count_by_size(2) << '\t' << u1.count_by_size(3) << '\t' << u1.count_by_size(4) << '\t' << u1 << std::endl;
		if(!u1.is_well())
			throw std::out_of_range("Received incorrect utf8 line.");
		for(auto it=u1.begin(); it!=u1.end(); it++)
		{
			tmp = (size_t) *it;
			if(tmp < 127) 
				++cnt;
		}
		if(cnt != u1.count_by_size(1))
			throw std::out_of_range("Incorrect behavior of count_by_size() method.");
	}
	return 0;
}
