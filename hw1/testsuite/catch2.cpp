#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>
#include "ustring.hpp"

// Модифицированный тест. Добавлена переменная u2
TEST_CASE("UString len check", "[length]")
{
	UString u1(U"﴾Измеряем длину текста для проверки﴿");
	REQUIRE(u1.length() == 36);
	UString u2("");
	REQUIRE(u2.length() == 0);
	u2 = u1;
	REQUIRE(u2.length() == 36);
	u1 = "";
	REQUIRE(u1.length() == 0);
}

TEST_CASE("UString reassign", "[std_string]")
{
	UString u1("123");
	REQUIRE(u1.length() == 3);
	u1 = "";
	REQUIRE(u1.length() == 0);
}

TEST_CASE("UString copy", "[copy]")
{
	UString u1("123");
	UString u2(u1);
	REQUIRE(u1 == u2);
}

// Тестируем наличие совпадения
TEST_CASE("UString compare", "[constructor_vs_assign]")
{
	UString u1("456");
	UString u2("");
	u2 = "456" ;
	REQUIRE( u1 == u2 );
	REQUIRE( u2.length() == 3 );
}

#ifdef	DEBUG

TEST_CASE("UString copy assignment", "[copy][assignment]")
{
	UString u1 = UString(U"777");
	REQUIRE( u1.debug_ == "Copy assignment UString& operator =(const std::u32string &str)" );
}

TEST_CASE("UString check iterator bounds", "[iterator_limits]")
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

TEST_CASE("UString get size", "[size]")
{
	UString u1(U"﴾Измеряем размер текста﴿");
	REQUIRE(u1.size() == 48);
	u1 = "";
	REQUIRE(u1.size() == 0);
}

TEST_CASE("UString add operator", "[add]")
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

TEST_CASE("UString non equal compare", "[not_equal]")
{
	UString u1(U"Строка");
	UString u2(U"Строка");
	u2.pop_back();
	REQUIRE(u1 != u2);
}

TEST_CASE("UString check push_back", "[push_back]")
{
	UString u1("");
	UString u2(U"เสน");
	u1.push_back("เ");
	u1.push_back("ส");
	u1.push_back("น");
	REQUIRE(u1 == u2);
}

TEST_CASE("UString check pop_back", "[pop_back]")
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

TEST_CASE("UString is_well", "[is_well]")
{
	UString u1(U"!#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~€<control>‚ƒ„…†‡ˆ‰Š‹ŒREVERSEŽSINGLEDEVICE‘’“”•–—˜™š›œOPERATINGžŸNO-BREAK¡¢£¤¥¦§¨©ª«¬­®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿĀāĂăĄąĆćĈĉĊċČčĎďĐđĒēĔĕĖėĘęĚěĜĝĞğĠġĢģĤĥĦħĨĩĪīĬĭĮįİıĲĳĴĵĶķĸĹĺĻļĽľĿŀŁłŃńŅņŇňŉŊŋŌōŎŏŐőŒœŔŕŖŗŘřŚśŜŝŞşŠšŢţŤťŦŧŨũŪūŬŭŮůŰűŲųŴŵŶŷŸŹźŻżŽžſƀƁƂƃƄƅƆƇƈƉƊƋƌƍƎƏƐƑƒƓƔƕƖƗƘƙƚƛƜƝƞƟƠơƢƣƤƥƦƧƨƩƪƫƬƭƮƯưƱƲƳƴƵƶƷƸƹƺƻƼƽƾƿǀǁǂǃǄǅǆǇǈǉǊǋǌǍǎǏǐǑǒǓǔǕǖǗǘǙǚǛǜǝǞǟǠǡǢǣǤǥǦǧǨǩǪǫǬǭǮǯǰǱǲǳǴǵǶǷǸǹǺǻǼǽǾǿȀȁȂȃȄȅȆȇȈȉȊȋȌȍȎȏȐȑȒȓȔȕȖȗȘșȚțȜȝȞȟȠȡȢȣȤȥȦȧȨȩȪȫȬȭȮȯȰȱȲȳȴȵȶȷȸȹȺȻȼȽȾȿɀɁɂɃɄɅɆɇɈɉɊɋɌɍɎɏɐɑɒɓɔɕɖɗɘəɚɛɜɝɞɟɠɡɢɣɤɥɦɧɨɩɪɫɬɭɮɯɰɱɲɳɴɵɶɷɸɹɺɻɼɽɾɿʀʁʂʃʄʅʆʇʈʉʊʋʌʍʎʏʐʑʒʓʔʕʖʗʘʙʚʛʜʝʞʟʠʡʢʣʤʥʦʧʨʩʪʫʬʭʮʯʰʱʲʳʴʵʶʷʸʹʺʻʼʽʾʿˀˁ˂˃˄˅ˆˇˈˉˊˋˌˍˎˏːˑ˒˓˔˕˖˗˘˙˚˛˜˝˞˟ˠˡˢˣˤ˥˦˧˨˩˪˫ˬ˭ˮ˯˰˱˲˳˴˵˶˷˸˹˺˻˼˽˾˿̴̵̶̷̸̡̢̧̨̛̖̗̘̙̜̝̞̟̠̣̤̥̦̩̪̫̬̭̮̯̰̱̲̳̹̺̻̼͇͈͉͍͎̀́̂̃̄̅̆̇̈̉̊̋̌̍̎̏̐̑̒̓̔̽̾̿̀́͂̓̈́͆͊͋͌̕̚ͅ͏͓͔͕͖͙͚͐͑͒͗͛ͣͤͥͦͧͨͩͪͫͬͭͮͯ͘͜͟͢͝͞͠͡ͰͱͲͳʹ͵Ͷͷͺͻͼͽ;Ϳ΄΅Ά·ΈΉΊΌΎΏΐΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩΪΫάέήίΰαβγδεζηθικλμνξοπρςστυφχψωϊϋόύώϏϐϑϒϓϔϕϖϗϘϙϚϛϜϝϞϟϠϡϢϣϤϥϦϧϨϩϪϫϬϭϮϯϰϱϲϳϴϵ϶ϷϸϹϺϻϼϽϾϿЀЁЂЃЄЅІЇЈ");
	REQUIRE( u1.is_well() );
}

TEST_CASE("UString check throw", "[no_invalid_chars_allowed]")
{
	INFO("Section with throws");
	REQUIRE_THROWS([&](){
		UString u1("\xff");
	}());
}

// Проверяем запись min/max символа utf для 1-4 байтов. Итого - 8 проверок
TEST_CASE("UString min max value check", "[min_max]")
{
	INFO("1-byte");
	UString u1("");
	u1 = "\x01";
	REQUIRE(u1.length() == 1);
	u1 = "\x7f";
	REQUIRE(u1.length() == 1);
	INFO("2-byte");
	u1 = "\xc0\x80";
	REQUIRE(u1.length() == 1);
	u1 = "\xdf\xbf";
	REQUIRE(u1.length() == 1);
	INFO("3-byte");
	u1 = "\xe0\x80\x80";
	REQUIRE(u1.length() == 1);
	u1 = "\xef\xbf\xbf";
	REQUIRE(u1.length() == 1);
	INFO("4-byte");
	u1 = "\xf0\x80\x80\x80";
	REQUIRE(u1.length() == 1);
	u1 = "\xf7\xbf\xbf\xbf";
	REQUIRE(u1.length() == 1);
}
TEST_CASE("UString iterator test", "[iterator]")
{
	UString u1(U"Текст для теста [iterator]");
	UString u2(U"ZZ[Текст для теста [iterator]]XX");
	auto i2 = u2.begin();
	++i2; // Пропустим первые 3 символа во второй строке
	++i2;
	++i2;
	// Постинкремент используется специально ради теста
	for( auto i = u1.begin(); i != u1.end(); i++ ) {
		REQUIRE(*i == *i2);
		++i2;
	}
}

// Новый тест. Проверка разыменованного значения при инкременте и декременте итератора
TEST_CASE("UString iterator compare", "[iterator_not_equal]")
{
	UString u1(U"Текст для теста [iterator_not_equal]");
	auto i1 = u1.begin();
	auto i2 = u1.begin();
	INFO("==");
	REQUIRE(i1 == i2);
	INFO("!=");
	++i1;
	REQUIRE(i1 != i2);
}

TEST_CASE("UString reverse iterator", "[reverse][iterator]")
{
	UString u1(U"Текст 2 для теста テキストをテストします。#ﻼ");
	UString u2(U"ﻼ#。すましトステをトスキテ атсет ялд 2 тскеТ");
	auto i2 = u2.end();
	for( auto i = u1.begin(); i != u1.end(); ++i ) {
		--i2;
		REQUIRE(*i == *i2);
	}
}

TEST_CASE("UString inc iterator", "[iterator][name_with_increment]")
{
	UString u1(U"** Текст 3 для теста **");
	auto i1 = u1.begin();
	REQUIRE( *i1 == *i1++ );
	REQUIRE( *i1 != *i1++ );
	i1 = u1.end();
	i1--;
	REQUIRE( *i1 == *i1-- );
	REQUIRE( *i1 != *i1-- );
}

TEST_CASE("UString output data", "[output]")
{
	UString u1(U"Текст для теста [output] テキストをテストします。#ﻼ ♂♀⚧👬👭𓀻 𓀼 𓀽 𓀾 𓀿 𓁀 𓁁 𓁂 𓁃 𓁄 𓁅 𓁆 𓁇 𓁈 𓁉 𓁊 𓁋 𓁌 𓁍 𓁎 𓁏");
	std::stringstream ss;
	ss << u1;
	REQUIRE(ss.str() == "Текст для теста [output] テキストをテストします。#ﻼ ♂♀⚧👬👭𓀻 𓀼 𓀽 𓀾 𓀿 𓁀 𓁁 𓁂 𓁃 𓁄 𓁅 𓁆 𓁇 𓁈 𓁉 𓁊 𓁋 𓁌 𓁍 𓁎 𓁏");
}
