// name: world.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "world.h"

#include "library.h"

#include "person.h"
#include "deposit.h"
#include "item.h"

#include "automata.h"

using namespace px;

namespace
{
	const unsigned int cell_length = 50;
	const point world_range(23, 1);
	const std::function<void(world::unit_ptr)> discard_fn([&](world::unit_ptr sink){});
}

const point world::cell_range(cell_length, cell_length);

world::world()
	:
	m_created(world_range, false),
	m_library(new library())
{
	rl::npc rat;
	rat.appearance({ 'r', 0x330000, 0.95f });
	rat.health() = 100;
	rat.tag("mob_r");

	m_library->push("mob_r", rat);

	rl::item ore;
	ore.appearance('o');
	ore.name("copper ore");
	ore.tag("ore_copper");
	m_library->push("ore_copper", ore);
}
world::~world() {}

world::map_ptr world::generate(const point &cell, std::function<void(world::unit_ptr)> fetch_fn)
{
	std::srand(cell.X + cell.Y * 911);

	world::map_ptr cell_map(new map_t(cell_range));

	bool sink = true;
	bool &created = m_created.at(cell, sink);

	if (cell.Y == 0)
	{
		generate_rail(*cell_map, created ? discard_fn : fetch_fn);
	}
	else
	{
		generate_wall(*cell_map, created ? discard_fn : fetch_fn);
	}

	if (!created)
	{
		created = true;
	}

	return cell_map;
}

void world::generate_rail(map_t &cell_map, std::function<void(unit_ptr)> fetch_fn)
{
	// generate
	automata<bool> walls(cell_range);
	walls.fill_indexed([](const point& p) { return std::rand() % 100 < 42; });
	walls.execute<unsigned int>([](unsigned int summ, bool element) { return summ + (element ? 1 : 0); }, 0, [](int summ) { return summ == 0 || summ >= 5; }, 4);

	// fill
	cell_range.enumerate([&](const point &position)
	{
		bool floor = (position.Y > 17 && position.Y < 22) || !walls.at(position) && position.Y != 0 && position.Y != cell_range.Y - 1;
		bool rail = position.Y == 19 || position.Y == 20;
		auto &t = cell_map.at(position);
		unsigned int glyph = rail ? '+' : floor ? '.' : ' ';

		t.appearance({ glyph, rail ? color(0.5, 0.6, 0.7) : floor ? color(0.2, 0.2, 0.2) : color(0.1, 0.1, 0.1) });
		t.transparent(floor);
		t.traversable(floor);
	});


	auto ore = std::make_shared<rl::item>(m_library->prototype<rl::item>("ore_copper"));

	world::unit_ptr vein(new rl::deposit(ore));
	vein->appearance({ 'O', 0xffffff });
	vein->position({ 12, 12 });
	fetch_fn(vein);
}

void world::generate_wall(map_t &cell_map, std::function<void(unit_ptr)> fetch_fn)
{
	// fill
	cell_map.range().enumerate([&](const point &position)
	{
		auto &t = cell_map.at(position);
		t.appearance({ ' ', color(0.1, 0.1, 0.1) });
		t.transparent(false);
		t.traversable(false);
	});
}

void world::store(world::unit_ptr unit)
{
	m_outher.emplace_back(unit);
}

void world::save(writer::node_ptr node) const
{
	auto outher = node->open("outher");
	for (auto unit : m_outher)
	{
		auto u = node->open("unit");
		u->write("tag", unit->tag());
		u->write("img", unit->appearance().image);
		u->write("color", unit->appearance().color);
		u->write("size", unit->appearance().size);
	}
}