// name: world.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "world.h"

#include "automata.h"

#include "person.h"
#include "deposit.h"
#include "item.h"

using namespace px;

namespace
{
	static const unsigned int cell_length = 25;
	static const point world_range(23, 1);
}

const point world::cell_range(cell_length, cell_length);

world::world() : m_created(world_range, false) {}
world::~world() {}

world::map_ptr world::generate(const point &cell, std::function<void(world::unit_ptr)> fetch_fn)
{
	world::map_ptr map(new map_t(cell_range));
	point offset = cell * cell_range;

	// generate
	automata<bool> walls(cell_range);
	std::srand(cell.X + cell.Y * 911);
	walls.fill_indexed([](const point& p) { return std::rand() % 100 < 42; });
	walls.execute<unsigned int>([](unsigned int summ, bool element) { return summ + (element ? 1 : 0); }, 0, [](int summ) { return summ == 0 || summ >= 5; }, 4);

	// fill
	cell_range.enumerate([&](const point &position)
	{
		bool floor = (position.Y > 5 && position.Y < 15) || !walls.at(position) && position.Y != 0 && position.Y != cell_range.Y - 1;
		bool rail = position.Y == 9 || position.Y == 10;
		auto &t = map->at(position);
		unsigned int glyph = rail ? '+' : floor ? '.' : ' ';

		t.appearance({ glyph, rail ? color(0.5, 0.6, 0.7) : floor ? color(0.2, 0.2, 0.2) : color(0.1, 0.1, 0.1) });
		t.transparent(floor);
		t.traversable(floor);
	});

	// units
	bool sink = true;
	bool &created = m_created.at(cell, sink);
	if (!created)
	{
		created = true;

		std::shared_ptr<rl::person> mob(new rl::person());
		mob->appearance({ 'r', 0xff0000, 0.5f });
		mob->position(offset + point(6, 6));
		mob->health() = 100;
		mob->energy() = { 0, 100 };
		fetch_fn(mob);

		rl::deposit::resource_ptr ore(new rl::item());
		ore->appearance('o');
		ore->name("copper ore");

		world::unit_ptr vein(new rl::deposit(ore));
		vein->appearance({ 'Q', 0xffff00 });
		vein->position(offset + point(12, 12));
		fetch_fn(vein);
	}

	return map;
}

void world::store(world::unit_ptr unit)
{

}