// name: entity.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "entity.h"

using namespace px;
using namespace px::rl;

// ctor & dtor

entity::entity()
{
}

entity::~entity()
{
}

entity::appearance_t entity::appearance() const
{
	return m_appearance;
}

void entity::appearance(entity::appearance_t appearance)
{
	m_appearance = appearance;
}

void entity::name(entity::name_t unit_name)
{
	m_name = unit_name;
}

entity::name_t entity::name() const
{
	return m_name;
}

void entity::tag(tag_t tag)
{
	m_tag = tag;
}

entity::tag_t entity::tag() const
{
	return m_tag;
}

void entity::faction(entity::faction_t faction_id)
{
	m_faction = faction_id;
}

int entity::reputation(entity::faction_t faction_id) const
{
	return m_faction == faction_id ? 100 : -100;
}

int entity::reputation(const entity &member) const
{
	return reputation(member.m_faction);
}