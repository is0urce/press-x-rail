// name: world.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "world.h"

#include "library.h"

#include "person.h"
#include "deposit.h"
#include "door.h"

#include "automata.h"
#include "vector.h"

using namespace px;

namespace
{
	const unsigned int cell_length = 50;
	const point world_range(23, 1);
	const world::fetch_op discard_fn([&](world::unit_ptr sink, point p){});
}

const point world::cell_range(cell_length, cell_length);

world::world()
	:
	m_created(world_range, false),
	m_landmarks(world_range),
	m_library(new library()),
	m_units(world_range)
{
	// landmarks
	m_landmarks.range().enumerate([this](const point &cell)
	{
		m_landmarks.at(cell) = [this](map_t &m, fetch_op f) { generate_rail(m, f); };
	});
	m_landmark_outer = [this](map_t &m, fetch_op f) { generate_wall(m, f); };

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

world::map_ptr world::generate(const point &cell, fetch_op fetch_fn)
{
	std::srand(cell.X + cell.Y * 911);

	world::map_ptr cell_map(new map_t(cell_range));
	point offset = cell * cell_range;

	bool sink = true;
	bool &created = m_created.at(cell, sink);

	// use generator for cell with offset unit placement function
	m_landmarks.at(cell, m_landmark_outer) // operator()
		(*cell_map, created ? discard_fn : [&](unit_ptr unit, point position)
	{
		fetch_fn(unit, position + offset);
	});

	// (re)store already generated units
	if (created)
	{
		for (auto unit : m_units.at(cell, m_units_outher))
		{
			fetch_fn(unit, unit->position());
		}
	}
	else
	{
		created = true;
	}

	return cell_map;
}

void world::generate_rail(map_t &cell_map, fetch_op fetch_fn)
{
	// generate
	automata<bool> walls(cell_range);
	walls.fill_indexed([](const point& p) { return std::rand() % 100 < 42; });
	walls.execute<unsigned int>([](unsigned int summ, bool element) { return summ + (element ? 1 : 0); }, 0, [](int summ) { return summ == 0 || summ >= 5; }, 4);

	// fill
	cell_map.range().enumerate([&](const point &position)
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

	std::shared_ptr<rl::deposit> vein(new rl::deposit(ore));
	vein->appearance({ 'O', 0xffffff }); 
	fetch_fn(vein, { 12, 12 });

	std::shared_ptr<rl::door> door(new rl::door());
	door->appearance({ ',', 0xffffff }, { '=', 0x333333 });
	fetch_fn(door, { 20, 20 });
}

void world::generate_wall(map_t &cell_map, fetch_op fetch_fn)
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
	m_units.at(cell(unit->position()), m_units_outher).emplace_back(unit);
}

void world::save(writer::node_ptr node) const
{
	auto outher = node->open("outher");
	for (auto unit : m_units_outher)
	{
		auto u = node->open("unit");
		u->write("tag", unit->tag());
		u->write("img", unit->appearance().image);
		u->write("color", unit->appearance().color);
		u->write("size", unit->appearance().size);
	}
}

point world::cell(const point &absolute) const
{
	return (vector(absolute) / cell_range).floor();
}