#include "stdafx.h"

#include "catch.hpp"

#include <px/writer.h>
#include <px/reader.h>

using namespace px;

TEST_CASE("io", "[io]")
{
	int random = std::rand();
	double f = 3.14;

	writer w("test.sav");
	w->write("version", 1); // int
	w->write("sizef", f); // float
	w->write("string0", "yohohoyo"); // string
	auto list = w->open("list"); // nested data
	list->write("item0", 1);
	list->write("item1", 2);
	w->write("inter", 0xff); // nesting ended, continue at top level
	w->write("random", random);

	// writing another value-size or opening another nested list at same level invalidates previous
	REQUIRE_THROWS_AS(list->write("item3", 3), std::logic_error);
	REQUIRE_THROWS_AS(w->write("thisstringistoolong", 3), std::logic_error);

	auto list2 = w->open("list2"); // anoter list
	list2->write("mob1", 1);
	list2->write("mob2", 2);
	list2->write("mob3", 3);
	list2->write("mob4", 4);

	auto l_n = w->open("n");
	l_n->write("before", 0);
	auto l_e = l_n->open("e");
	auto l_s = l_e->open("s");
	auto l_t = l_s->open("t");
	l_t->write("egg", 64);
	l_e->write("after", 0);

	w->open("empty");

	w->write("ending", 64);

	w.end();

	REQUIRE_THROWS_AS(reader("not_exists.sav"), std::runtime_error);

	reader r("test.sav");

	int guess;
	REQUIRE_NOTHROW(r["random"].read(guess));
	REQUIRE(guess == random); // int value
	REQUIRE_NOTHROW(r["inter"] >> guess);
	REQUIRE(guess == 0xff);

	double float_value;
	REQUIRE_NOTHROW(r["sizef"] >> float_value);
	REQUIRE(float_value == f); // float falue

	std::string yoyo;
	REQUIRE_NOTHROW(r["string0"] >> yoyo);
	REQUIRE(yoyo == "yohohoyo"); // std::string

	REQUIRE_THROWS(r["string0"] >> guess); // type mismatch

	int x;

	r["list2"]["mob4"] >> x;
	REQUIRE(x == 4); // last nesting
	r["list2"]["mob1"] >> x;
	REQUIRE(x == 1); // first nested

	r["n"]["e"]["s"]["t"]["egg"] >> x;
	REQUIRE(x == 64); // long nesting

	r["ending"] >> x;
	REQUIRE(x == 64); // last overall

	// list enumerate
	int s = 0;
	r["list2"].enumerate([&](reader::node list_node)
	{
		s += list_node.read<int>();
	});
	REQUIRE(s == 10); // summ of nodes values

	// empty list enumerate
	r["empty"].enumerate([&](reader::node list_node)
	{
		throw std::runtime_error("no nodes");
	});
}