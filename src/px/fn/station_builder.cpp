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
		void draw_square(map<_V> &map, point start, point range, _V tile)
		{
			range.enumerate(start, [&](const point &p) { map.at(p) = tile; });
		}
		template <typename _V>
		void draw_horisontal(map<_V> &map, point start, unsigned int size, _V tile)
		{
			for (unsigned int i = 0; i < size; ++i)
			{
				map.at(start) = tile;
				++start.X;
			}
		}
		template <typename _V>
		void draw_vertical(map<_V> &map, point start, unsigned int size, _V tile)
		{
			for (unsigned int i = 0; i < size; ++i)
			{
				map.at(start) = tile;
				++start.Y;
			}
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
			point room(1, rail_h + 4);
			point room_size(w / 4 - 2, 11);
			draw_square(walls, { w / 4, rail_h }, { w / 4 * 2, 10 }, false);
			draw_square(walls, room, room_size, false);
			draw_horisontal(walls, { room.X, room.Y + 3 }, 5, true);
			draw_horisontal(walls, { room.X, room.Y + 7 }, 5, true);
			draw_vertical(walls, { room.X + 5, room.Y + 2 }, 3, true);
			draw_vertical(walls, { room.X + 5, room.Y + 6 }, 3, true);
			draw_vertical(walls, { room.X + 5, room.Y + 10 }, 3, true);
			walls.at({ room.X + 5, room.Y + 0 }) = true;

			//fetch_fn(m_library->make<rl::unit>("lantern"), { room.X + 5, room.Y + 3 });
			//fetch_fn(m_library->make<rl::unit>("lantern"), { room.X + 5, room.Y + 7 });

			point door_pos(room.X + room_size.X, room.Y + 1);
			walls.at(door_pos) = false;
			fetch_fn(m_library->make<rl::door>("door"), door_pos);

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

			cell_map.at(door_pos).appearance().color = { 0.15, 0.15, 0.15 };
		}
	}
}
