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
				point pos;
				do
				{
					pos = { std::rand() % cell_map.width(), std::rand() % cell_map.height() };
				}
				while (walls.at(pos));
				fetch_fn(vein, pos);
			}
			
			auto chest = std::make_shared<rl::container>();
			chest->add_item(m_library->make<rl::item>("ore_copper"));
			chest->appearance({ 'c', 0xffff00 });
			fetch_fn(chest, { 21, 21 });
		}
	}
}
