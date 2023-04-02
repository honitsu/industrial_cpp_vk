#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "ustring.hpp"
#include <iostream>
                               
#ifndef __linux__ 
#include <locale>
#include <windows.h>
#endif

TEST_CASE("UString", "[length]")
{
	UString u1("﴾Измеряем длину текста﴿");
	REQUIRE(u1.length() == 23ul);
	u1 = "";
	REQUIRE(u1.length() == 0ul);
}

TEST_CASE("UString", "[size]")
{
	UString u1("﴾Измеряем размер текста﴿");
	REQUIRE(u1.size() == 48ul);
	u1 = "";
	REQUIRE(u1.size() == 0ul);
}

TEST_CASE("UString", "[push_back]")
{
	UString u1("");
	UString u2("เสน");
	u1.push_back("เ");
	u1.push_back("ส");
	u1.push_back("น");
	REQUIRE(u1 == u2);
}

TEST_CASE("UString", "[pop_back]")
{
	UString u1("АБ𓂸");
	UString u2("АБ𓂸스");
	u2.pop_back();
	REQUIRE(u1 == u2);
	u1 = "";
	u2 = "";
	u2.pop_back();
	REQUIRE(u1 == u2);
}

TEST_CASE("UString", "[is_well][!shouldfail]")
{
	UString u1("\xff");
	INFO("Section 1");
	CHECK(u1.is_well());
	u1 = "Корректный UTF-текст";
	INFO("Section 2");
	CHECK(u1.is_well());
}

TEST_CASE("UString", "[iterator]")
{
	UString u1("Текст для теста [iterator]");
	UString u2("ZZ[Текст для теста [iterator]]XX");
	auto i2 = u2.begin();
	i2++; // Пропустим первые 3 символа во второй строке
	i2++;
	i2++;
	for( auto i = u1.begin(); i != u1.end(); i++ ) {
		REQUIRE(*i == *i2);
		i2++;
	}
}

TEST_CASE("UString", "[reverse][iterator]")
{
	UString u1("Текст 2 для теста テキストをテストします。#ﻼ");
	UString u2("ﻼ#。すましトステをトスキテ атсет ялд 2 тскеТ");
	auto i2 = u2.end();
	for( auto i = u1.begin(); i != u1.end(); i++ ) {
		--i2;
		REQUIRE(*i == *i2);
	}
}


TEST_CASE("UString", "[output]")
{
	UString u1("Текст для теста [output] テキストをテストします。#ﻼ ♂♀⚧👬👭𓀻 𓀼 𓀽 𓀾 𓀿 𓁀 𓁁 𓁂 𓁃 𓁄 𓁅 𓁆 𓁇 𓁈 𓁉 𓁊 𓁋 𓁌 𓁍 𓁎 𓁏");
	std::stringstream ss;
	ss << u1;
	REQUIRE(ss.str() == "Текст для теста [output] テキストをテストします。#ﻼ ♂♀⚧👬👭𓀻 𓀼 𓀽 𓀾 𓀿 𓁀 𓁁 𓁂 𓁃 𓁄 𓁅 𓁆 𓁇 𓁈 𓁉 𓁊 𓁋 𓁌 𓁍 𓁎 𓁏");
}
