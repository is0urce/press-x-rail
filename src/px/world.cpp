// name: world.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "world.h"

#include <px/library.h>

#include <px/fn/delegate_builder.h>
#include <px/fn/cave_builder.h>
#include <px/fn/station_builder.h>

#include <px/fn/automata.h>
#include <px/vector.h>

using namespace px;

namespace
{
	const unsigned int cell_length = 50;
	const point world_range(23, 1);
	const world::builder_t::fetch_op discard_fn([&](world::unit_ptr sink, point p){});
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
		auto &generator = m_landmarks.at(cell);
		if (cell.X % 2 == 0)
		{
			generator.reset(new fn::cave_builder(m_library));
		}
		else
		{
			generator.reset(new fn::station_builder(m_library));
		}
	});
	m_landmark_outer.reset(new fn::delegate_builder<map_t, unit_ptr>([this](map_t &m, builder_t::fetch_op f) { generate_wall(m, f); }));

	rl::npc rat;
	rat.appearance({ 'r', 0x330000, 0.95f });
	rat.health() = 100;
	rat.tag("mob_r");
	m_library->push("mob_r", rat);

	rl::item ore;
	ore.appearance('o');
	ore.name("copper ore");
	ore.tag("ore_copper");
	ore.stackable(true);
	m_library->push("ore_copper", ore);

	rl::unit lantern;
	lantern.appearance({ ' ', color(1, 1, 1) });
	lantern.light({ { 3, 3, 3 }, true });
	m_library->push("lantern", lantern);

	rl::door door;
	door.appearance({ ' ', 0x333333 }, { '+', 0x333333 });
	m_library->push("door", door);
}
world::~world() {}

world::map_ptr world::generate(const point &cell, builder_t::fetch_op fetch_fn)
{
	std::srand(cell.X + cell.Y * 911);

	world::map_ptr cell_map(new map_t(cell_range));
	point offset = cell * cell_range;

	bool sink = true;
	bool &created = m_created.at(cell, sink);

	// use generator for cell with offset unit placement function
	auto &generator = m_landmarks.at(cell, m_landmark_outer);
	generator->build(*cell_map, created ? discard_fn : [&](unit_ptr unit, point position)
	{
		fetch_fn(unit, position + offset);
	});

	// (re)store already generated units
	if (created)
	{
		auto &list = m_units.at(cell, m_units_outher);
		for (auto unit : list)
		{
			fetch_fn(unit, unit->position());
		}
		list.clear();
	}
	else
	{
		created = true;
	}

	return cell_map;
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

void world::generate_wall(map_t &cell_map, builder_t::fetch_op fetch_fn)
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