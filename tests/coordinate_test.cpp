#include "stdafx.h"

#include "catch.hpp"

#include "vector.h"
#include "point.h"

using namespace px;

TEST_CASE("Points", "[coordinates]")
{
	int ax = std::rand() % 1000 - 500;
	int ay = std::rand() % 1000 - 500;
	int bx = std::rand() % 1000 - 500;
	int by = std::rand() % 1000 - 500;

	point a(ax, ay);
	point b(bx, by);
	point copy_a(a);
	point zero;
	point summ(ax + bx, ay + by);

	REQUIRE(zero.X == 0);
	REQUIRE(zero.Y == 0);
	REQUIRE(a.X == ax);
	REQUIRE(a.Y == ay);
	REQUIRE(a == copy_a);

	REQUIRE(a.moved(b) == summ);
	REQUIRE(a == copy_a);

	SECTION("move")
	{
		a.move(b);
		a == summ;
	}
}