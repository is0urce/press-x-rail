// name: station_builder.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "station_builder.h"

#include <px/library.h>

#include <px/point.h>
#include <px/map.h>

#include <px/rl/deposit.h>
#include <px/rl/door.h>

#include <px/fn/automata.h>

namespace px
{
	namespace
	{
		template <typename _V>
		inline void draw_square(map<_V> &map, point start, point range, _V rect_value)
		{
			range.enumerate(start, [&](const point &p) { map.at(p) = rect_value; });
		}
	}
	namespace fn
	{
		station_builder::station_builder(lib_ptr lib) : m_library(lib) {}
		station_builder::~station_builder() {}

		void station_builder::generate(map_t &cell_map, fetch_op fetch_fn)
		{
			point range = cell_map.range();
			int rail_h = 19; // rails Y
			int w = range.X;

			map<bool> walls(range);
			walls.fill(true);
			draw_square(walls, { w / 4, rail_h }, { w / 4 * 2, 10 }, false);
			draw_square(walls, { 1, rail_h + 1 }, { w / 4, 10 }, false);
			walls.at({ w / 4 - 1, rail_h + 5 }) = false;

			// fill
			cell_map.range().enumerate([&](const point &position)
			{
				bool wall = walls.at(position);
				bool floor = (position.Y > rail_h - 2 && position.Y < rail_h + 3) || !wall && position.Y != 0 && position.Y != range.Y - 1;
				bool rail = position.Y == rail_h || position.Y == rail_h + 1;
				unsigned int glyph = rail ? 8212 : floor ? '.' : ' ';

				auto &t = cell_map.at(position);
				t.appearance({ glyph, rail ? color(0.5, 0.5, 0.5) : floor ? color(0.2, 0.2, 0.2) : color(0.1, 0.1, 0.1) });
				t.transparent(floor);
				t.traversable(floor);
			});
		}
	}
}
