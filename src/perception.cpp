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
	static const color black_color(0);
}

perception::perception(point range) :
	m_appearance(range)
{
	init(range, {});
}

perception::perception(point range, point start) :
	m_appearance(range),
	m_start(start)
{
	init(range, start);
}

void perception::init(point range, point start)
{
	m_version = 0;
	m_start = start;
	m_ground.reset(new map<perception::ground_t>(range));
	m_ground_prev.reset(new map<perception::ground_t>(range));
	m_color.reset(new map<color>(range));
	m_color_prev.reset(new map<color>(range));
	m_hide.reset(new map<bool>(range));
	m_hide_prev.reset(new map<bool>(range));
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

unsigned int perception::version() const
{
	return m_version;
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
	point prev = position - m_move;
	return m_color_prev->in_range(prev) ? m_color_prev->at(prev) : black_color; // _color->getelement(position); // to fade in
}

const perception::ground_t& perception::ground(const point &position) const
{
	return m_ground->at(position);
}

const perception::ground_t& perception::ground_previous(const point &position) const
{
	point prev = position - m_move;
	return m_ground_prev->in_range(prev) ? m_color_prev->at(prev) : black_color; // m_ground->at(position);
}

bool perception::hide(const point &position) const
{
	return m_hide->at(position);
}

void perception::appearance(const point &position, const perception::appearance_t &tile)
{
	m_appearance.at(position) = tile;
	++m_version;
}

void perception::light(const point &position, const color& light_value)
{
	m_color->at(position) = light_value;
	++m_version;
}

void perception::ground(const point &position, const perception::ground_t &ground_value)
{
	m_ground->at(position) = ground_value;
	++m_version;
}

void perception::add_unit(perception::appearance_t appearance, point position, point position_previous)
{
	m_units.emplace_back(appearance, position - m_start, position_previous - m_start_prev);
	bool sink;
	m_hide->at(position - m_start, sink) = true;
	++m_version;
}

void perception::enumerate_units(perception::enum_fn fn) const
{
	if (!fn) throw std::logic_error("perception::enumerate_units - fn is null");

	std::for_each(m_units.begin(), m_units.end(), fn);
}

perception::unit_list::size_type perception::unit_count() const
{
	return m_units.size();
}

void perception::start(point start)
{
	m_start = start;
	++m_version;
}

point perception::start() const
{
	return m_start;
}

point perception::start_previous() const
{
	return m_start_prev;
}

point perception::movement() const
{
	return m_move;
}

void perception::swap(const point& start)
{
	m_units.clear();
	m_notifications.clear();

	std::swap(m_ground, m_ground_prev);
	std::swap(m_color, m_color_prev);

	std::swap(m_hide, m_hide_prev);
	m_hide->fill(false);

	std::swap(m_start, m_start_prev);
	m_start = start;
	m_move = m_start - m_start_prev;

	++m_version;
}

void perception::add_notification(notification::string_t text, color c, const point &position)
{
	m_notifications.emplace_back(text, c, position - m_start);
	++m_version;
}

void perception::enumerate_notifications(std::function<void(const notification&)> fn) const
{
	std::for_each(m_notifications.begin(), m_notifications.end(), fn);
}