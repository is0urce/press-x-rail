﻿// name: world.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "world.h"

#include <px/library.h>
#include <px/rl/serializer.h>

#include <px/fn/delegate_builder.h>
#include <px/fn/cave_builder.h>
#include <px/fn/station_builder.h>

#include <px/fn/automata.h>
#include <px/vector.h>

namespace px
{

	namespace
	{
		const unsigned int cell_length = 50;
		const point world_range(23, 1);
		const world::builder_t::fetch_op discard_fn([&](world::unit_ptr sink, point p){});
	}

	const point world::cell_range(cell_length, cell_length);

	world::world(library_ptr lib)
		:
		m_library(lib),
		m_created(world_range, false),
		m_landmarks(world_range),
		m_units(world_range)
	{
		if (!lib) throw std::logic_error("px::world::ctor(library) library is null");

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

	void world::save(writer::node_ptr node, const rl::serializer& s) const
	{
		auto outher = node->open("outher");
		for (auto unit : m_units_outher)
		{
			s.save(unit, outher);
		}
	}
	void world::load(const reader::node &node, const rl::serializer& s)
	{

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
}