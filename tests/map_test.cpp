#include "stdafx.h"
//
//#include "catch.hpp"
//
//#include "../src/map.h"
//
//using namespace px;
//
//TEST_CASE("map", "[map]")
//{
//	int w = std::rand() % 20 + 1;
//	int h = std::rand() % 20 + 1;
//
//	int element = 100500;
//	point range(w, h);
//	point start;
//
//	// constructors
//	map<int> a(range);
//	map<int> b(w, h);
//	map<int> c(w, h, element);
//	map<int> d(range, element + 1);
//	map<int> e(range, [=](point p) { return element + 2; });
//	REQUIRE_THROWS(map<int> f1(0, h));
//	REQUIRE_THROWS(map<int> f2(w, 0));
//
//	// attributes
//	REQUIRE(a.width() == w);
//	REQUIRE(a.height() == h);
//	REQUIRE(a.range() == range);
//
//	// access
//	REQUIRE_THROWS(a.at(point(0, -1)));
//	REQUIRE_THROWS(a.at(point(-1, 0)));
//	REQUIRE_THROWS(a.at(point(0, h)));
//	REQUIRE_THROWS(a.at(point(w, h)));
//
//	REQUIRE(c.at(start) == element);
//	REQUIRE(d.at(start) == element + 1);
//	REQUIRE(e.at(start) == element + 2);
//
//	REQUIRE(a.at(point(-1, -1), element + 1) == element + 1);
//
//	// resize
//	REQUIRE_THROWS(a.resize(point(0, 0)));
//	a.resize(3, 3);
//	a.resize(point(50, 100));
//	REQUIRE(a.width() == 50);
//	REQUIRE(a.height() == 100);
//
//	// swap
//	c.swap(a);
//	REQUIRE(c.width() == 50);
//	REQUIRE(a.width() == w);
//	REQUIRE(a.at(start) == element);
//
//	a.fill(element + 4);
//	REQUIRE(a.at(start) == element + 4);
//	a.fill([=]() { return element + 5; });
//	REQUIRE(a.at(start) == element + 5);
//	REQUIRE_THROWS(a.fill<int>(c, [](int e) { return 0; })); // size mismatch
//}