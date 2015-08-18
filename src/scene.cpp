// name: scene.cpp
// type: c++ source file
// desc: 'scene' class implementation
// auth: is0urce

#include "stdafx.h"

#include "scene.h"

#include "unit.h"

using namespace px;
using namespace px::rl;

namespace
{
	static const int cell_width = 100;
	static const int cell_height = cell_width;
	static const point cell_range(cell_width, cell_height);
}

scene::scene() :
	m_map(new map<tile_t>(cell_range)),
	m_units([](const point& a, const point& b) { return std::tie(a.X, a.Y) < std::tie(b.X, b.Y); })
{
}

scene::~scene()
{
}

scene::tile_t& scene::tile(const point& point)
{
	return m_map->in_range(point) ? m_default : m_map->at(point);
}

const scene::tile_t& scene::tile(const point& point) const
{
	return m_map->in_range(point) ? m_default : m_map->at(point);
}

bool scene::transparent(const point& point) const
{
	if (!tile(point).transparent()) return false;

	auto find = m_units.find(point);
	if (find == m_units.end()) return true;

	return find->second->transparent();

	//return readtile(point).istransparent();

	//auto find = _objects.find(point);
	//// serch forward
	//for (auto it = find; it != _objects.end(); ++it)
	//{
	//	if (it->second->getposition() != point) break; // leaving key range
	//	if (!it->second->istransparent())
	//	{
	//		return false;
	//	}
	//}
	//// backward
	//for (auto it = find; it != _objects.begin(); /**/ )
	//{
	//	--it;

	//	if (it->second->getposition() != point) break; // leaving key range
	//	if (!it->second->istransparent())
	//	{
	//		return false;
	//	}
	//}
	return true;
}

bool scene::traversable(const point& point) const
{
	if (!tile(point).traversable()) return false;

	auto find = m_units.find(point);
	if (find == m_units.end()) return true;

	return find->second->traversable();
}

bool scene::traversable(const point& point, unsigned int layer) const
{
	if (!tile(point).traversable(layer)) return false;

	auto find = m_units.find(point);
	if (find == m_units.end()) return true;

	return find->second->traversable();
}

void scene::add(unit_ptr unit, const point& position)
{
	unit->position(position);
	m_units.emplace(position, unit);
}

void scene::move(unit_ptr unit, const point& position)
{
	remove(unit);
	add(unit, position);
}

void scene::remove(unit_ptr unit)
{
	auto find = m_units.find(unit->position());
	if (find == m_units.end()) throw std::logic_error("assert 1: scene::remove unit not found or position invalid");
	if (find->second != unit) throw std::logic_error("assert 2: scene::remove unit position invalid or scene corrupted");
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

scene::unit_ptr scene::blocking(const point& place) const
{
	auto hint = m_units.find(place);
	return hint == m_units.end() ? nullptr : hint->second;
}