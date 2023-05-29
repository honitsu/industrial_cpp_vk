#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "ustring.hpp"

TEST_CASE("UString", "[length]")
{
	UString u1(U"﴾Измеряем длину текста﴿");
	REQUIRE(u1.length() == 23);
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

TEST_CASE("UString", "[size]")
{
	UString u1(U"﴾Измеряем размер текста﴿");
	REQUIRE(u1.size() == 48);
	u1 = "";
	REQUIRE(u1.size() == 0);
}

TEST_CASE("UString", "[add]")
{
	INFO("+=");
	UString u1(U"Складываем ");
	UString u2(U"строки.");
	UString u3(U"Складываем строки.");
	u1 += u2;
	REQUIRE(u1 == u3);
	INFO("u1 + u2");
	u1 = u1 + u2;
	u3 = U"Складываем строки.строки.";
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
	UString u1("\xff");
	INFO("Section 1");
	REQUIRE_FALSE(u1.is_well());
	u1 = U"Корректный UTF-текст";
	INFO("Section 2");
	CHECK(u1.is_well());
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

TEST_CASE("UString", "[output]")
{
	UString u1(U"Текст для теста [output] テキストをテストします。#ﻼ ♂♀⚧👬👭𓀻 𓀼 𓀽 𓀾 𓀿 𓁀 𓁁 𓁂 𓁃 𓁄 𓁅 𓁆 𓁇 𓁈 𓁉 𓁊 𓁋 𓁌 𓁍 𓁎 𓁏");
	std::stringstream ss;
	ss << u1;
	REQUIRE(ss.str() == "Текст для теста [output] テキストをテストします。#ﻼ ♂♀⚧👬👭𓀻 𓀼 𓀽 𓀾 𓀿 𓁀 𓁁 𓁂 𓁃 𓁄 𓁅 𓁆 𓁇 𓁈 𓁉 𓁊 𓁋 𓁌 𓁍 𓁎 𓁏");
}
