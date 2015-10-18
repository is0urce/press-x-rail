// name: person.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_PERSON_H
#define PX_PERSON_H

#include <px/rl/actor.h>
#include <px/rl/character.h>
#include <px/rl/equipment.h>

#include "px/delegate_action.h"
#include "px/ability.h"
#include "px/user_ability.h"

#include "px/rl/effect.h"

namespace px
{
	namespace rl
	{
		class person : 
			public actor, 
			public character,
			public equipment
		{
		public:
			typedef person caster_t;
			typedef std::shared_ptr<unit> target_t;
			typedef delegate_action<caster_t*, target_t> action_t;

			typedef ability<target_t> ability_t; // value-type to save intristic values - cd
			typedef std::unique_ptr<ability_t> ability_ptr;

		protected:
			std::vector<action_t> m_skills;

			// ctor & dtor
		public:
			person();
			virtual ~person();

		public:
			static std::string signature();

		protected:
			virtual void apply_effect(effect &e) override;
			virtual std::string sign_unit() const override;
			virtual void serialize(unit::o_node node, const serializer &s) const override;
			virtual void deserialize(unit::i_node node, const serializer &s) override;
			virtual bool useable_unit(const environment&, user_t user) const override;
			virtual void action_unit(environment&) override;

		public:
			ability_ptr skill(unsigned int slot);
			void add_skill(action_t skill);
		};
	}
}

#endif