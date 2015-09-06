// name: unit.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "unit.h"

using namespace px;
using namespace px::rl;

// ctor & dtor

unit::unit() : m_remove(false), m_invincible(false)
{
}

unit::unit(const point &position) : m_position(position), m_prev_position(position), m_remove(false), m_invincible(false)
{
}

unit::~unit()
{
}

// virtual methods

bool unit::traversable_unit() const
{
	return false;
}

bool unit::transparent_unit() const
{
	return true;
}

bool unit::useable_unit(user_t user) const
{
	return false;
}

void unit::use_unit(user_t user)
{
}

void unit::apply_effect(effect &e)
{
}

void unit::serialize(writer::node_ptr node) const
{
	point m_position;
	point m_prev_position;
	bool m_remove;
	bool m_invincible;
	node->write("pos_x", m_position.X);
	node->write("pos_y", m_position.Y);
	node->write("remove", m_remove);
	node->write("static", m_invincible);
}

void unit::deserialize(const reader::node &node)
{
	node["pos_x"] >> m_position.X;
	node["pos_y"] >> m_position.Y;
	node["remove"] >> m_remove;
	node["static"] >> m_invincible;
}

// methods

void unit::save(writer::node_ptr node) const
{
	serialize(node);
}
void unit::load(const reader::node &node)
{
	deserialize(node);
}

bool unit::traversable() const
{
	return traversable_unit();
}

bool unit::transparent() const
{
	return transparent_unit();
}

bool unit::useable(user_t user) const
{
	return useable_unit(user);
}

void unit::use(user_t user)
{
	use_unit(user);
}

const point& unit::position() const
{
	return m_position;
}

const point& unit::previous_position() const
{
	return m_prev_position;
}

void unit::store_position()
{
	m_prev_position = m_position;
}

void unit::position(const point& position)
{
	m_position = position;
}

void unit::position(const point& position, const point& previous)
{
	m_position = position;
	m_prev_position = previous;
}

void unit::destroy()
{
	m_remove = true;
}

bool unit::destroying() const
{
	return m_remove;
}

bool unit::invincible() const
{
	return m_invincible;
}

void unit::invincible(bool is_invincible)
{
	m_invincible = is_invincible;
}

void unit::apply(effect &e)
{
	apply_effect(e);
}