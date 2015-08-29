#include "stdafx.h"

#include "catch.hpp"

#include "../src/writer.h"

#include <iostream>

using namespace px;

TEST_CASE("writer", "[writer]")
{
	writer w("test.sav");
	w.top()->write("version", 1); // int
	w.top()->write("size", 10000.0); // float
	w.top()->write("string0", "yohohoyo"); // string
	auto list = w.top()->open("list"); // nested data
	list->write("item0", 1);
	list->write("item1", 2);
	w.top()->write("inter", 0xff); // nesting ended, continue at top level

	// writing another value-size or opening another nested list at same level invalidates previous
	REQUIRE_THROWS_AS(list->write("item3", 3), std::logic_error);
	REQUIRE_THROWS_AS(list->write("thisstringistoolong", 3), std::logic_error);

	auto list2 = w.top()->open("list2"); // anoter list
	list2->write("mob1", 1);
	list2->write("mob2", 2);
	list2->write("mob3", 3);
	list2->write("mob4", 4);
}