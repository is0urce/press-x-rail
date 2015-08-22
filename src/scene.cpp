// name: scene.cpp
// type: c++ source file
// desc: 'scene' class implementation
// auth: is0urce

#include "stdafx.h"

#include "scene.h"

#include "unit.h"
#include "vector.h"

using namespace px;
using namespace px::rl;

namespace
{
	static const unsigned int cell_width = 100;
	static const unsigned int cell_height = cell_width;
	static const point cell_range(cell_width, cell_height);
}

scene::scene()
	:
	m_units([](const point &a, const point &b) { return std::tie(a.X, a.Y) < std::tie(b.X, b.Y); })
{
	focus({ 0, 0 }, true);
}

scene::~scene()
{
}

scene::tile_t& scene::tile(const point &position)
{
	return m_map->at(position, m_default);
}

const scene::tile_t& scene::tile(const point &position) const
{
	return m_map->at(position, m_default);
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

void scene::remove(unit_ptr unit)
{
	if (!unit) throw new std::logic_error("scene::remove - unit is null");

	auto find = m_units.find(unit->position());
	if (find == m_units.end()) throw std::logic_error("scene::remove assert #1: scene::remove unit not found or position invalid");
	if (find->second != unit) throw std::logic_error("scene::remove assert #2: scene::remove unit position invalid or scene corrupted");
	m_units.erase(find);
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
	auto i = m_units.begin();
	auto end = m_units.end();
	while (i != end)
	{
		fn(i->second);
		++i;
	}
}

scene::unit_ptr scene::blocking(const point& place) const
{
	auto hint = m_units.find(place);
	return hint == m_units.end() ? nullptr : hint->second;
}

void scene::focus(point center, bool force)
{
	point focus = (vector(center) / cell_range).floor();
	if (force || m_focus != focus)
	{
		m_focus = focus;
		m_map.swap(m_world.generate(m_focus, [this](unit_ptr unit) { add(unit); }));
	}
}

void scene::focus(point center)
{
	focus(center, false);
}