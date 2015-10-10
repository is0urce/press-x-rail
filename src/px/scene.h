// name: scene.h
// type: c++ header
// desc: 'scene' class definition
// auth: is0urce

#ifndef PX_SCENE_H
#define PX_SCENE_H

#include <px/tile.h>
#include <px/world.h>

#include <px/reader.h>
#include <px/writer.h>

#include <memory>
#include <map>

namespace px
{
	class scene
	{
	public:
		typedef world::tile_t tile_t;
		typedef world::map_t map_t;
		typedef std::unique_ptr<map_t> map_ptr;
		typedef world::unit_ptr unit_ptr;
		typedef std::multimap<point, unit_ptr, std::function<bool(const point&, const point&)>> unit_list;
		typedef int timer_t;

		// attributes
	private:
		world m_world;
		map<map_ptr> m_maps;
		tile_t m_default;
		point m_focus;
		unit_list m_units;

		// ctor & dtor
	public:
		scene();
		virtual ~scene();
	private:
		scene(const scene&) = delete; // disable copy

	private:
		map_t* select_map(const point &cell) const;

	public:
		// tiles
		tile_t& tile(const point &position);
		const tile_t& tile(const point &position) const;
		// units
		unit_ptr blocking(const point &position) const;
		const unit_list& units() const;
		void add(unit_ptr unit);
		void add(unit_ptr unit, const point &position);
		void move(unit_ptr unit, const point &position);
		void remove(unit_ptr unit);
		void clear();
		unsigned int count() const;
		void enumerate_units(std::function<void(unit_ptr)> fn) const;
		// both
		bool transparent(const point &position) const;
		bool traversable(const point &position) const;
		bool traversable(const point &position, unsigned int layer) const;

		void tick(timer_t ticks);
		void focus(point absolute);
		void focus(point absolute, bool force);

		void save(writer::node_ptr node);
		void load(const reader::node &node);
	};
}

#endif