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

void character::store(writer::node_ptr node) const
{
	node->write("hp", m_hp);
	node->write("mp", m_mp);
	auto stats = node->open("stats");
	for (unsigned int i = 0, len = m_base.size(); i < len; ++i)
	{
		node->write(io::to_key(i), m_base[i]);
	}
}
void character::restore(const reader::node &node)
{
	node["hp"] >> m_hp;
	node["mp"] >> m_mp;
	unsigned int index = 0;
	node["stats"].enumerate([&](reader::node stat_node)
	{
		stat_node.read(m_base[index]);
		++index;
	});
	if (index != m_base.size()) throw std::runtime_error("px::character::restore stats size mismatch");
}

character::resource_t& character::health() { return m_hp; }
const character::resource_t& character::health() const { return m_hp; }
character::resource_t& character::energy() { return m_mp; }
const character::resource_t& character::energy() const { return m_mp; }

bool character::dead() const { return m_hp.empty(); }