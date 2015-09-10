// name: game.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "appearance.h"
#include "tile.h"
#include "map.h"
#include "writer.h"
#include "reader.h"

#include <memory>
#include <list>

namespace px
{
	class library;
	namespace rl
	{
		class unit;
	}

	class world
	{
	public:
		typedef appearance<unsigned int> appearance_t;
		typedef tile<appearance_t> tile_t;
		typedef map<tile_t> map_t;
		typedef std::shared_ptr<rl::unit> unit_ptr;
		typedef std::unique_ptr<map_t> map_ptr;

	public:
		static const point cell_range;

	private:
		map<bool> m_created;
		std::list<unit_ptr> m_outher;
		std::unique_ptr<library> m_library;

	public:
		world();
		world(reader::node &node);
		~world();

	public:
		map_ptr generate(const point &cell, std::function<void(unit_ptr)> fetch_fn);
		void generate_wall(map_t &cell_map, std::function<void(unit_ptr)> fetch_fn);
		void generate_rail(map_t &cell_map, std::function<void(unit_ptr)> fetch_fn);
		void store(unit_ptr unit);

		void save(writer::node_ptr node) const;
		void load(reader::node &node);
	};
}