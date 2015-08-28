#include "stdafx.h"

#include "catch.hpp"

#include "../src/writer.h"

#include <iostream>

using namespace px;

TEST_CASE("writer", "[writer]")
{
	writer w("test.sav");
	w.top()->write({ "int_646" }, 0xf0);
	w.top()->write({ "elevat7" }, 10000.0);
	w.top()->write({ "zomg" }, "yohohoyo");
	auto small = w.top()->open({ "slist" });
	small->write({ "version" }, 1);
	auto list = w.top()->open({ "list" });
	list->write({ "item0" }, 1);
	list->write({ "item1" }, 2);
	w.top()->write({ "inter" }, 0xff);
	REQUIRE_THROWS_AS(list->write({ "item3" }, 3), std::logic_error);
	auto list2 = w.top()->open({ "list2" });
	list2->write({ "mob1" }, 1);
	list2->write({ "mob2" }, 3);
	list2->write({ "mob3" }, 7);
	list2->write({ "mob4" }, 15);
}