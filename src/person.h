// name: person.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "actor.h"
#include "character.h"

#include "delegate_action.h"
#include "ability.h"
#include "user_ability.h"

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
			typedef ability<target_t> ability_t;
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
				unit::serialize(node);
				character::save(node);
			}
			virtual void deserialize(const reader::node &node) override
			{
				unit::deserialize(node);
				character::load(node);
			}

		public:
			ability_ptr skill(unsigned int slot)
			{
				return (slot < m_skills.size()) ? ability_ptr(new user_ability<user_t*, target_t>(this, &m_skills.at(slot))) : nullptr;
			}
		};
	}
}