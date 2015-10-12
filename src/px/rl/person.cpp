// name: person.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "person.h"

#include <px/rl/serializer.h>
#include <px/library.h>

namespace px
{
	namespace rl
	{
		person::person() {}
		person::~person() {}

		std::string person::signature() { return "person"; }

		void person::apply_effect(effect &e)
		{
			e.apply(*this);
		}
		std::string person::sign_unit() const  { return signature(); }
		void person::serialize(o_node node, const serializer &s) const
		{
			auto skills = node->open("skills");
			for (auto &skill : m_skills)
			{
				skills->write("tag", skill.tag());
			}
			unit::serialize(node, s);
			character::store(node);
		}
		void person::deserialize(const i_node &node, const serializer &s)
		{
			m_skills.clear();
			node["skills"].enumerate([&](i_node skill_node)
			{
				add_skill(s.library()->prototype<action_t>(skill_node.read()));
			});
			unit::deserialize(node, s);
			character::restore(node);
		}

		person::ability_ptr person::skill(unsigned int slot)
		{
			return slot < m_skills.size() ?
				ability_ptr(new user_ability<caster_t*, target_t>(this, &m_skills.at(slot)))
				:
				nullptr;
		}
		void person::add_skill(action_t skill)
		{
			m_skills.push_back(skill);
		}
	}
}