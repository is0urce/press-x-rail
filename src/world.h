// name: game.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "appearance.h"
#include "tile.h"
#include "map.h"

#include <memory>

namespace px
{
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
		typedef unsigned int save_slot;

	public:
		static const point cell_range;

	private:
		map<bool> m_created;

	public:
		world();
		world(save_slot load);
		~world();

	public:
		map_ptr generate(const point &cell, std::function<void(unit_ptr)> fetch_fn);
		void store(unit_ptr unit);

		void save(save_slot repository);
		void load(save_slot repository);
	};
}