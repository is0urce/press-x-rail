// name: perception.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "perception.h"

#include "unit.h"

using namespace px;
using namespace px::shell;

namespace
{
	const color black_color(0);
}

perception::perception(const point& range)
	:
	m_appearance(range),
	m_ground(new map<perception::ground_t>(range)),
	m_ground_prev(new map<perception::ground_t>(range)),
	m_color(new map<color>(range)),
	m_color_prev(new map<color>(range))
{
}

perception::~perception()
{
}

int perception::width() const
{
	return m_appearance.width();
}

int perception::height() const
{
	return m_appearance.height();
}

const point& perception::range() const
{
	return m_appearance.range();
}

bool perception::in_range(const point &point) const
{
	return m_appearance.in_range(point);
}

const perception::appearance_t& perception::appearance(const point &position) const
{
	return m_appearance.at(position);
}

const color& perception::light(const point &position) const
{
	return m_color->at(position);
}

const color& perception::light_previous(const point &position) const
{
	point prev = point();// position + _movement;
	return m_color_prev->in_range(prev) ? m_color_prev->at(prev) : black_color; // _color->getelement(position); // to fade in
}

const perception::ground_t& perception::ground(const point &position) const
{
	return m_ground->at(position);
}

void perception::appearance(const point &position, const perception::appearance_t &tile)
{
	m_appearance.at(position) = tile;
}

void perception::light(const point &position, const color& color)
{
	m_color->at(position) = color;
}