// name: world.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "world.h"

#include "unit.h"

using namespace px;

namespace
{
	static const unsigned int cell_length = 100;
}

const point world::cell_range(cell_length, cell_length);

world::world() {}
world::~world() {}

world::map_ptr world::generate(const point &cell, std::function<void(world::unit_ptr)> fetch_fn)
{
	auto map = world::map_ptr(new map_t(cell_range));
	cell_range.enumerate([&](const point &position)
	{
		bool rail = position.Y == 9 || position.Y == 10;
		auto &t = map->at(position);
		t.appearance({ rail ? (unsigned int)'+' : '.', rail ? color(0.5, 0.6, 0.7) : color(0.2, 0.2, 0.2) });
		t.transparent(rail);
		t.traversable(rail);
	});

	world::unit_ptr mob(new rl::unit());
	mob->appearance('g');
	mob->position({ 6, 6 });
	fetch_fn(mob);

	return map;
}