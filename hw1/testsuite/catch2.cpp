#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "ustring.hpp"

TEST_CASE("UString", "[length]")
{
	UString u1(U"﴾Измеряем длину текста для проверки﴿");
	REQUIRE(u1.length() == 36);
	u1 = "";
	REQUIRE(u1.length() == 0);
}

TEST_CASE("UString", "[std_string]")
{
	UString u1("123");
	REQUIRE(u1.length() == 3);
	u1 = "";
	REQUIRE(u1.length() == 0);
}

// Тестируем наличие совпадения
TEST_CASE("UString", "[constructor_vs_assign]")
{
	UString u1("456");
	UString u2("");
	u2 = "456";
	REQUIRE( u1 == u2 );
}

#ifdef	DEBUG
TEST_CASE("UString", "[same_assignment]")
{
	UString u1("abc");
	UString u2(u1);
	u2 = "abc";
	REQUIRE( u1 == u2 );
	REQUIRE( u2.debug_ == "Skip assigment of the same text." );
	UString u3("");
	REQUIRE( u3.debug_ == "Skip assigment of the same text in constructor." );
}
#endif

TEST_CASE("UString", "[size]")
{
	UString u1(U"﴾Измеряем размер текста﴿");
	REQUIRE(u1.size() == 48);
	u1 = "";
	REQUIRE(u1.size() == 0);
}

TEST_CASE("UString", "[add]")
{
	UString u1(U"Строка ");
	UString u2(U"хорошая.");
	UString u3(U"Строка хорошая.");
	u1 += u2;
	REQUIRE(u1 == u3);
	u1 = u1 + u2;
	u3 = U"Строка хорошая.хорошая.";
	REQUIRE(u1 == u3);
}

TEST_CASE("UString", "[not_equal]")
{
	UString u1(U"Строка");
	UString u2(U"Строка");
	u2.pop_back();
	REQUIRE(u1 != u2);
}

TEST_CASE("UString", "[push_back]")
{
	UString u1("");
	UString u2(U"เสน");
	u1.push_back("เ");
	u1.push_back("ส");
	u1.push_back("น");
	REQUIRE(u1 == u2);
}

TEST_CASE("UString", "[pop_back]")
{
	UString u1(U"АБ𓂸");
	UString u2(U"АБ𓂸스");
	u2.pop_back();
	REQUIRE(u1 == u2);
	u1 = "";
	u2 = "";
	u2.pop_back();
	REQUIRE(u1 == u2);
}

TEST_CASE("UString", "[is_well]")
{
// Из-за того, что был переписан конструктор, больше нельзя присвоить строке неверное значение
// В текущей реализации невозможна ситуация, когда is_well возвращает False
/*
	UString u1("\xff");
	INFO("Section 1");
	REQUIRE_FALSE(u1.is_well());
	u1 = U"Корректный UTF-текст";
	INFO("Section 2");
	CHECK(u1.is_well());
*/
	INFO("Section with throws");
	REQUIRE_THROWS([&](){
		UString u1("\xff");
	}());
}

TEST_CASE("UString", "[iterator]")
{
	UString u1(U"Текст для теста [iterator]");
	UString u2(U"ZZ[Текст для теста [iterator]]XX");
	auto i2 = u2.begin();
	++i2; // Пропустим первые 3 символа во второй строке
	++i2;
	++i2;
	for( auto i = u1.begin(); i != u1.end(); i++ ) {
		REQUIRE(*i == *i2);
		i2++;
	}
}

TEST_CASE("UString", "[iterator_not_equal]")
{
	UString u1(U"Текст для теста [iterator_not_equal]");
	auto i1 = u1.begin();
	auto i2 = u1.begin();
	INFO("==");
	REQUIRE(i1 == i2);
	INFO("!=");
	i1++;
	REQUIRE(i1 != i2);
}

TEST_CASE("UString", "[reverse][iterator]")
{
	UString u1(U"Текст 2 для теста テキストをテストします。#ﻼ");
	UString u2(U"ﻼ#。すましトステをトスキテ атсет ялд 2 тскеТ");
	auto i2 = u2.end();
	for( auto i = u1.begin(); i != u1.end(); i++ ) {
		--i2;
		REQUIRE(*i == *i2);
	}
}

#ifdef DEBUG
TEST_CASE("UString", "[iterator_limits]")
{
	UString u1(U"Текст для теста [iterator_limits] --");
	UString u2(U"Текст для теста [iterator_limits] ++");
	auto i1 = u1.begin();
	auto i2 = u2.end();
	--i1;
	REQUIRE(u1.debug_ == "Ignore an attempt to go beyond begin().");
	++i2;
	REQUIRE(u2.debug_ == "Ignore an attempt to go beyond end().");
}
#endif

TEST_CASE("UString", "[output]")
{
	UString u1(U"Текст для теста [output] テキストをテストします。#ﻼ ♂♀⚧👬👭𓀻 𓀼 𓀽 𓀾 𓀿 𓁀 𓁁 𓁂 𓁃 𓁄 𓁅 𓁆 𓁇 𓁈 𓁉 𓁊 𓁋 𓁌 𓁍 𓁎 𓁏");
	std::stringstream ss;
	ss << u1;
	REQUIRE(ss.str() == "Текст для теста [output] テキストをテストします。#ﻼ ♂♀⚧👬👭𓀻 𓀼 𓀽 𓀾 𓀿 𓁀 𓁁 𓁂 𓁃 𓁄 𓁅 𓁆 𓁇 𓁈 𓁉 𓁊 𓁋 𓁌 𓁍 𓁎 𓁏");
}
