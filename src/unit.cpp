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

unit::unit(const point& position) : m_position(position), m_prev_position(position), m_remove(false)
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

bool unit::useable_unit() const
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

bool unit::useable() const
{
	return useable_unit();
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

void unit::position_remember()
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

unit::appearance_t unit::appearance() const
{
	return m_appearance;
}

void unit::appearance(appearance_t appearance)
{
	m_appearance = appearance;
}

void unit::destroy()
{
	m_remove = true;
}

bool unit::destroying() const
{
	return m_remove;
}

void unit::name(unit::name_t unit_name)
{
	m_name = unit_name;
}

unit::name_t unit::name() const
{
	return m_name;
}

void unit::faction(unit::faction_t faction_id)
{
	m_faction = faction_id;
}

int unit::reputation(unit::faction_t faction_id) const
{
	return m_faction == faction_id ? 100 : -100;
}

int unit::reputation(const unit &member) const
{
	return reputation(member.m_faction);
}