// name: person.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include <px/rl/actor.h>
#include <px/rl/character.h>

#include "px/delegate_action.h"
#include "px/ability.h"
#include "px/user_ability.h"

#include "effect.h"

namespace px
{
	namespace rl
	{
		class person : 
			public actor, 
			public character
		{
		public:
			typedef person user_t;
			typedef std::shared_ptr<unit> target_t;
			typedef delegate_action<user_t*, target_t> action_t;

			typedef ability<target_t> ability_t; // value-type to save intristic values - cd
			typedef std::unique_ptr<ability_t> ability_ptr;

		protected:
			std::vector<action_t> m_skills;

			// ctor & dtor
		public:
			person() {}
			virtual ~person() {}

		protected:
			virtual void apply_effect(effect &e) override { e.apply(*this); }
			virtual void serialize(writer::node_ptr node) const override
			{
				auto l = node->open("person");
				unit::serialize(l);
				character::store(l->open("char"));
				//auto skills = l->open("skills");
				//for (auto &skill : m_skills)
				//{
				//	skills->write("skill", skill.tag());
				//}
			}
			virtual void deserialize(const reader::node &node) override
			{
				unit::deserialize(node["unit"]);
				character::restore(node["char"]);
				//l["skills"].enumerate([&](const reader::node &skill_node)
				//{
				//	std::string tag = skill_node["tag"].read<std::string>();
				//});
			}

		public:
			ability_ptr skill(unsigned int slot)
			{
				return slot < m_skills.size() ?
					ability_ptr(new user_ability<user_t*, target_t>(this, &m_skills.at(slot)))
					:
					nullptr;
			}
			void add_skill(action_t skill)
			{
				m_skills.push_back(skill);
			}
		};
	}
}