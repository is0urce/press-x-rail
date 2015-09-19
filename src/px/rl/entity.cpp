// name: entity.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "entity.h"

// ctor & dtor

namespace px
{
	namespace rl
	{
		entity::entity() : m_faction(0)
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

		void entity::store(writer::node_ptr node) const
		{
			node->write("name", m_name);
			node->write("tag", m_tag);
			node->write("appear", m_appearance);
			node->write("faction", m_faction);
			node->write("light", m_light);
		}

		void entity::restore(const reader::node &node)
		{
			node["name"] >> m_name;
			node["tag"] >> m_tag;
			node["appear"] >> m_appearance;
			node["faction"] >> m_faction;
			node["light"] >> m_light;
		}

		const entity::light_t& entity::light() const
		{
			return m_light;
		}
		entity::light_t& entity::light()
		{
			return m_light;
		}
		void entity::light(light_t light_source)
		{
			m_light = light_source;
		}
	}
}