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
		typedef std::list<unit_ptr> unit_list;
		typedef std::unique_ptr<map_t> map_ptr;
		typedef std::function<void(unit_ptr, point)> fetch_op; // std::function<void(unit_ptr, point)>
		typedef std::function<void(map_t&, fetch_op)> landmark_t;

	public:
		static const point cell_range;

	private:
		std::unique_ptr<library> m_library;
		map<bool> m_created;

		map<landmark_t> m_landmarks;
		landmark_t m_landmark_outer;

		map<unit_list> m_units;
		unit_list m_units_outher;

	public:
		world();
		world(reader::node &node);
		~world();
	private:
		world(const world&) = delete;

	public:
		point cell(const point &absolute) const;

		map_ptr generate(const point &cell, fetch_op fetch);
		void generate_wall(map_t &cell_map, fetch_op fetch);
		void generate_rail(map_t &cell_map, fetch_op fetch);
		void store(unit_ptr unit);

		void save(writer::node_ptr node) const;
		void load(reader::node &node);
	};
}