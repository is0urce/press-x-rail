// name: station_builder.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "cave_builder.h"

#include <px/library.h>

#include <px/rl/deposit.h>
#include <px/rl/container.h>

#include <px/fn/automata.h>

namespace px
{
	namespace fn
	{
		cave_builder::cave_builder(lib_ptr lib) : m_library(lib) {}
		cave_builder::~cave_builder() {}

		void cave_builder::generate(map_t &cell_map, fetch_op fetch_fn)
		{
			point range = cell_map.range();
			int h = 19; // rails Y

			// generate
			automata<bool> walls(range);
			walls.fill_indexed([](const point& p) { return std::rand() % 100 < 42; });
			walls.execute<unsigned int>([](unsigned int summ, bool element) { return summ + (element ? 1 : 0); }, 0, [](int summ) { return summ == 0 || summ >= 5; }, 4);

			// fill
			range.enumerate([&](const point &position)
			{
				bool wall = walls.at(position);
				bool floor = (position.Y > h - 2 && position.Y < h + 3) || !wall && position.Y != 0 && position.Y != range.Y - 1;
				bool rail = position.Y == h || position.Y == h + 1;
				unsigned int glyph = rail ? 8212 : floor ? '.' : ' ';

				auto &t = cell_map.at(position);
				t.appearance({ glyph, rail ? color(0.5, 0.5, 0.5) : floor ? color(0.2, 0.2, 0.2) : color(0.1, 0.1, 0.1) });
				t.transparent(floor);
				t.traversable(floor);
			});

			for (unsigned int n = 0; n < 10; ++n)
			{
				auto vein = std::make_shared<rl::deposit>(m_library->make<rl::item>("ore_copper"));
				vein->appearance({ 'O', 0xffffff });
				vein->name("copper ore vein");
				point pos;
				do
				{
					pos = { std::rand() % cell_map.width(), std::rand() % cell_map.height() };
				}
				while (walls.at(pos));
				fetch_fn(vein, pos);
			}

			for (unsigned int n = 0; n < 10; ++n)
			{
				auto flora = m_library->make<rl::deposit>("fireflower");
				point pos;
				do
				{
					pos = { std::rand() % cell_map.width(), std::rand() % cell_map.height() };
				} while (walls.at(pos));
				fetch_fn(flora, pos);
			}

			for (unsigned int n = 0; n < 10; ++n)
			{
				auto flora = m_library->make<rl::deposit>("iceflower");
				point pos;
				do
				{
					pos = { std::rand() % cell_map.width(), std::rand() % cell_map.height() };
				} while (walls.at(pos));
				fetch_fn(flora, pos);
			}

			for (unsigned int n = 0; n < 10; ++n)
			{
				auto flora = m_library->make<rl::deposit>("felflower");
				point pos;
				do
				{
					pos = { std::rand() % cell_map.width(), std::rand() % cell_map.height() };
				} while (walls.at(pos));
				fetch_fn(flora, pos);
			}
			
			auto chest = std::make_shared<rl::container>();
			chest->appearance({ 'c', 0xffff00 });
			chest->name("wooden chest");
			chest->invincible(true);
			chest->add_item(m_library->make<rl::item>("ore_copper"));
			fetch_fn(chest, { 21, 21 });

			fetch_fn(m_library->make<rl::npc>("mob_rat"), { 22, 21 });
		}
	}
}
