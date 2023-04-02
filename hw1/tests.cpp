#include "ustring.hpp"
#include <string>
#include <gtest/gtest.h>
                               
#ifndef __linux__ 
#include <locale>
#include <windows.h>
#endif

TEST (UString, length)
{
	UString u1("﴾Измеряем длину текста﴿");
	ASSERT_EQ(u1.length(), 23ul);
	u1 = "";
	ASSERT_EQ(u1.length(), 0ul);
}

TEST (UString, size)
{
	UString u1("﴾Измеряем размер текста﴿");
	ASSERT_EQ(u1.size(), 48ul);
	u1 = "";
	ASSERT_EQ(u1.size(), 0ul);
}

TEST (UString, push_back)
{
	UString u1("");
	UString u2("เสน");
	u1.push_back("เ");
	u1.push_back("ส");
	u1.push_back("น");
	ASSERT_EQ(u1,u2);
}

TEST (UString, pop_back)
{
	UString u1("АБ");
	UString u2("АБ스");
	u2.pop_back();
	ASSERT_EQ(u1,u2);
	u1 = "";
	u2 = "";
	u2.pop_back();
	ASSERT_EQ(u1,u2);
}

TEST (UString, is_well)
{
	UString u1("\xff");
	ASSERT_FALSE(u1.is_well());
	u1 = "Правильный UTF-текст";
	ASSERT_TRUE(u1.is_well());
}

TEST (UString, iterator)
{
	UString u1("Текст 2 для теста");
	UString u2("ZZ[Текст 2 для теста]XX");
	auto i2 = u2.begin();
	i2++; // Пропустим первые 3 символа во второй строке
	i2++;
	i2++;
	for( auto i = u1.begin(); i != u1.end(); i++ ) {
		ASSERT_EQ(*i, *i2);
		i2++;
	}
}

TEST (UString, iterator_reverse)
{
	UString u1("Текст 2 для теста テキストをテストします。#ﻼ");
	UString u2("ﻼ#。すましトステをトスキテ атсет ялд 2 тскеТ");
	auto i2 = u2.end();
	for( auto i = u1.begin(); i != u1.end(); i++ ) {
		--i2;
		ASSERT_EQ(*i, *i2);
	}
}

TEST(UString, output)
{
	UString u1("Текст 3 для теста テキストをテストします。#ﻼ");
	std::stringstream ss;
	ss << u1;
	ASSERT_EQ(ss.str(), "Текст 3 для теста テキストをテストします。#ﻼ");
}



int main(int argc, char **argv)
{
	//std::cout << argc << std::endl;
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
