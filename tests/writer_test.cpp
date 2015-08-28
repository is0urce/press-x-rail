#include "stdafx.h"

#include "catch.hpp"

#include "../src/writer.h"

#include <iostream>

using namespace px;

TEST_CASE("writer", "[writer]")
{
	writer<int, float> w("test.sav");
	w.top()->write('int_', 63);
	w.top()->write('zomg', "yohohoyo");
}