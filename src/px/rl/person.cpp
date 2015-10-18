// name: person.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "person.h"

#include <px/rl/serializer.h>
#include <px/rl/visitor.h>
#include <px/library.h>

namespace px
{
	namespace rl
	{
		namespace
		{
			template <typename _T>
			bool status_expired(const status<_T>& s)
			{
				return s.expired();
			}
		}

		person::person() {}
		person::~person() {}

		std::string person::signature() { return "person"; }

		void person::accept_visitor(visitor &v)
		{
			v.expose(*this);
		}
		std::string person::sign_unit() const  { return signature(); }
		void person::serialize(unit::o_node node, const serializer &s) const
		{
			auto skills = node->open("skills");
			for (auto &skill : m_skills)
			{
				skills->write("tag", skill.tag());
			}
			unit::serialize(node, s);
			character::store(node);
		}
		void person::deserialize(unit::i_node node, const serializer &s)
		{
			m_skills.clear();
			node["skills"].enumerate([&](unit::i_node skill_node)
			{
				add_skill(s.library()->prototype<action_t>(skill_node.read()));
			});
			unit::deserialize(node, s);
			character::restore(node);
		}

		bool person::useable_unit(const environment&, user_t user) const
		{
			return false;// reputation(*user) < 0;
		}

		void person::action_unit(environment &env)
		{
			actor::action_unit(env);
			tick(1);
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

		void person::add_status(status_t affect, bool silent)
		{
			m_affect.push_back(affect);
			if (!silent)
			{
				affect.on_apply(*this);
			}
		}
		void person::add_status(status_t affect)
		{
			add_status(affect, false);
		}
		void person::tick(status_t::timer_t span)
		{
			for (auto &s : m_affect)
			{
				s.on_tick(*this, span);
				if (s.expired())
				{
					s.on_expire(*this);
				}
			}
			m_affect.erase(std::remove_if(m_affect.begin(), m_affect.end(), status_expired<rl::person>), m_affect.end());
		}
		void person::enumerate_affects(std::function<void(status_t&)> enum_fn)
		{
			std::for_each(m_affect.begin(), m_affect.end(), enum_fn);
		}
	}
}