// name: scene.cpp
// type: c++ source file
// desc: 'scene' class implementation
// auth: is0urce

#include "stdafx.h"

#include "scene.h"

#include <px/writer_node.hpp>
#include <px/reader.h>
#include <px/rl/unit.h>
#include <px/rl/serializer.h>

namespace px
{
	namespace
	{
		static const unsigned int sight_reach = 1;
		static const point sight_range(sight_reach * 2 + 1, sight_reach * 2 + 1);
		static const point sight_center(sight_reach, sight_reach);
	}

	scene::scene(std::shared_ptr<world> world)
		:
		m_world(world),
		m_maps(sight_range),
		m_units([](const point &a, const point &b) { return std::tie(a.X, a.Y) < std::tie(b.X, b.Y); })
	{
		if (!world) throw std::logic_error("px::scene::ctor(world) world is null");

		focus({ 0, 0 }, true);
	}

	scene::~scene()
	{
	}

	scene::tile_t& scene::tile(const point &position)
	{
		point c = m_world->cell(position);
		auto *map = select_map(c);
		return map ? map->at(position - c * world::cell_range) : m_default;
	}

	const scene::tile_t& scene::tile(const point &position) const
	{
		point c = m_world->cell(position);
		auto *map = select_map(c);
		return map ? map->at(position - c * world::cell_range) : m_default;
	}

	bool scene::transparent(const point &point) const
	{
		if (!tile(point).transparent()) return false;

		auto find = m_units.find(point);
		if (find == m_units.end()) return true;

		return find->second->transparent();

		return true;
	}

	bool scene::traversable(const point &point) const
	{
		if (!tile(point).traversable()) return false;

		auto find = m_units.find(point);
		if (find == m_units.end()) return true;

		return find->second->traversable();
	}

	bool scene::traversable(const point &point, unsigned int layer) const
	{
		if (!tile(point).traversable(layer)) return false;

		auto find = m_units.find(point);
		if (find == m_units.end()) return true;

		return find->second->traversable();
	}

	void scene::add(unit_ptr unit)
	{
		if (!unit) throw new std::logic_error("scene::add - unit is null");

		add(unit, unit->position());
	}

	void scene::add(unit_ptr unit, const point &position)
	{
		if (!unit) throw new std::logic_error("scene::add - unit is null");

		unit->position(position);
		m_units.emplace(position, unit);
	}

	void scene::move(unit_ptr unit, const point &position)
	{
		if (!unit) throw new std::logic_error("scene::move - unit is null");

		remove(unit);
		add(unit, position);
	}

	void scene::move(unit_ptr::element_type &unit, const point &position)
	{
		//if (!element_this) throw std::runtime_error("px::scehe::select(unit*) - unit ptr is null");

		auto range = m_units.equal_range(unit.position());
		auto first = range.first, last = range.second;

		for (auto i = first; i != last; ++i)
		{
			if (i->second.get() == &unit)
			{
				move(i->second, position);
				return;
			}
		}
	}

	void scene::remove(unit_ptr unit)
	{
		if (!unit) throw new std::logic_error("px::scene::remove(unit) - unit is null");

		auto range = m_units.equal_range(unit->position());
		auto i = range.first, last = range.second;
		bool erased = false;
		while (i != last)
		{
			if (i->second == unit)
			{
				m_units.erase(i);
				erased = true;
				break;
			}
			else
			{
				++i;
			}
		};
		if (!erased)
		{
			throw std::logic_error("px::scene::remove(..) assert #1: scene::remove unit not found or position invalid");
		}
	}

	void scene::clear()
	{
		m_units.clear();
	}

	unsigned int scene::count() const
	{
		return m_units.size();
	}

	void scene::tick(scene::timer_t ticks)
	{
	}

	const scene::unit_list& scene::units() const
	{
		return m_units;
	}

	void scene::enumerate_units(std::function<void(scene::unit_ptr)> fn) const
	{
		for (auto i = m_units.begin(), end = m_units.end(); i != end; ++i)
		{
			fn(i->second);
		}
	}

	scene::unit_ptr scene::blocking(const point& place) const
	{
		auto range = m_units.equal_range(place);
		for (auto it = range.first, last = range.second; it != last; ++it)
		{
			if (!it->second->traversable())
			{
				return it->second;
			}
		};

		return nullptr;
	}

	void scene::focus(point absolute, bool force)
	{
		point focus = m_world->cell(absolute);
		if (force || m_focus != focus)
		{
			m_focus = focus;

			// generate neighbour maps
			sight_range.enumerate([&](const point &range_point)
			{
				point cell = m_focus + range_point - sight_center;
				m_maps.at(range_point).swap(m_world->generate(cell, [&](unit_ptr unit, point position)
				{
					add(unit, position);
					unit->store_position();
				}));
			});

			// store out-of-range units back in world
			for (auto i = m_units.begin(), end = m_units.end(); i != end;)
			{
				if (!select_map(m_world->cell(i->first)))
				{
					m_world->store(i->second);
					i = m_units.erase(i);
				}
				else
				{
					++i;
				}
			}
		}
	}

	void scene::focus(point absolute)
	{
		focus(absolute, false);
	}

	scene::map_t* scene::select_map(const point &cell) const
	{
		return m_maps.at(cell - m_focus + sight_center, nullptr).get();
	}

	void scene::save(writer::node_ptr node, const rl::serializer& s)
	{
		if (!node) throw std::logic_error("px:scene::save(..) - node is null");

		auto units_node = node->open("units");
		enumerate_units([&](unit_ptr u_ptr)
		{
			s.save(u_ptr, units_node);
		});
	}

	void scene::load(const reader::node &node, const rl::serializer& s)
	{
		clear();

		node["units"].enumerate([&](reader::node n)
		{
			auto u = s.load(n);
			u->store_position();
			add(u);
		});
	}
}