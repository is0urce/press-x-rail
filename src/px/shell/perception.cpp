// name: perception.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "perception.h"

#include <px/rl/unit.h>

#include <numeric>

using namespace px;
using namespace px::shell;

namespace
{
	const color black_color(0);
	const double notify_size = 0.39; // "a bit" smaller than tiles (1-phi)
	inline bool alpanum_less(const point &a, const point &b) { return std::tie(a.X, a.Y) < std::tie(b.X, b.Y); }
}

perception::perception(point range) :
	m_appearance(range),
	m_notify(alpanum_less)
{
	init(range, {});
}

perception::perception(point range, point start) :
	m_appearance(range),
	m_notify(alpanum_less),
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
	point prev = position + m_move;
	return m_color_prev->in_range(prev) ? m_color_prev->at(prev) : black_color; // _color->getelement(position); // to fade in
}

const perception::ground_t& perception::ground(const point &position) const
{
	return m_ground->at(position);
}

const perception::ground_t& perception::ground_previous(const point &position) const
{
	point prev = position + m_move;
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

void perception::add_avatar(appearance_t appearance, point position, point position_previous, light_t light_source, bool hide)
{
	m_units.emplace_back(appearance, light_source, position - m_start, position_previous - m_start_prev);
	if (hide)
	{
		bool sink;
		m_hide->at(position - m_start, sink) = true;
	}
	++m_version;
}

void perception::enumerate_avatars(perception::enum_fn fn) const
{
	if (!fn) throw std::logic_error("perception::enumerate_units - fn is null");

	std::for_each(m_units.begin(), m_units.end(), fn);
}

perception::unit_list::size_type perception::avatar_count() const
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
	m_notify.clear();
	m_projectiles.clear();

	std::swap(m_ground, m_ground_prev);
	std::swap(m_color, m_color_prev);

	std::swap(m_hide, m_hide_prev);
	m_hide->fill(false);

	std::swap(m_start, m_start_prev);
	m_start = start;
	m_move = m_start - m_start_prev;

	++m_version;
}

void perception::add_notification(notification::string_t text, color c, const point &position, double multiplier)
{
	point pos = position - m_start;
	auto range = m_notify.equal_range(pos);
	double elevation = std::accumulate(range.first, range.second, (double)pos.Y, [](double &a, const std::pair<point, notification> &e){ return (std::max)(a, e.second.position.Y + e.second.size); });
	m_notify.emplace_hint(range.first, pos, notification(text, c, vector(pos.X, elevation), notify_size * multiplier));
	++m_version;
}

void perception::add_notification(notification::string_t text, color c, const point &position)
{
	add_notification(text, c, position, 1.0);
}

void perception::enumerate_notifications(std::function<void(const notification&)> fn) const
{
	std::for_each(m_notify.begin(), m_notify.end(), [&](const std::pair<point, notification> &p){ fn(p.second); });
}

void perception::add_projectile(projectile particle)
{
	m_projectiles.push_back(particle);
}

void perception::enumerate_projectiles(std::function<void(const projectile&)> fn) const
{
	std::for_each(m_projectiles.begin(), m_projectiles.end(), fn);
}

unsigned int perception::projectile_count() const
{
	return m_projectiles.size();
}