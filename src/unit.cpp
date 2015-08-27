// name: Unit.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "unit.h"

using namespace px;
using namespace px::rl;

// ctor & dtor

unit::unit() : m_remove(false)
{
}

unit::unit(const point &position) : m_position(position), m_prev_position(position), m_remove(false)
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

// methods

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