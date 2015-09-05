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

character::resource_t& character::health() { return m_hp; }
const character::resource_t& character::health() const { return m_hp; }
character::resource_t& character::energy() { return m_mp; }
const character::resource_t& character::energy() const { return m_mp; }

bool character::dead() const { return m_hp.empty(); }