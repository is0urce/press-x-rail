// name: scene.h
// type: c++ header
// desc: 'scene' class definition
// auth: is0urce

#pragma once

#include "tile.h"
#include "map.h"

#include <memory>
#include <map>

namespace px
{
	namespace rl
	{
		class unit;

		class scene
		{
		public:
			typedef tile<unsigned int, 1> tile_t;
			typedef std::shared_ptr<unit> unit_ptr;
			typedef std::map<point, unit_ptr, std::function<bool(const point&, const point&)>> unit_list;
			typedef std::function<void(unit_ptr)> enum_fn;
			typedef int timer_t;

			// attributes
		private:
			tile_t m_default;
			std::unique_ptr<map<tile_t>> m_map;
			unit_list m_units;

			// ctor & dtor
		public:
			scene();
			virtual ~scene();
		private:
			scene(const scene &none); // disable copy

			// methods
		public:
			tile_t& tile(const point &position);
			const tile_t& tile(const point &position) const;
			const unit_list& units() const;
			void add(unit_ptr unit, const point& position);
			void move(unit_ptr unit, const point& position);
			void remove(unit_ptr unit);
			void clear();
			unsigned int count() const;
			void tick(timer_t ticks);
			void enumerate_units(enum_fn fn) const;
			bool transparent(const point &position) const;
			bool traversable(const point &position) const;
			bool traversable(const point &position, unsigned int layer) const;
			unit_ptr blocking(const point &position) const;
		};
	}
}