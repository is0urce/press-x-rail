// name: character.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "character.h"

using namespace px;
using namespace px::rl;

namespace
{
	const unsigned int max_attribute = (unsigned int)attribute::max_attribute;
}

character::character() : m_base(max_attribute), m_computed(max_attribute) {}
character::~character() {}

void character::save(writer::node_ptr node) const
{
	node->write("hp", m_hp);
	node->write("mp", m_mp);
	node->write("stats", m_base);
}
void character::load(const reader::node &node)
{
	node["hp"] >> m_hp;
	node["mp"] >> m_mp;
	node["stats"] >> m_base;
}

character::resource_t& character::health() { return m_hp; }
const character::resource_t& character::health() const { return m_hp; }
character::resource_t& character::energy() { return m_mp; }
const character::resource_t& character::energy() const { return m_mp; }

bool character::dead() const { return m_hp.empty(); }