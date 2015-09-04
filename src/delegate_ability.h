// name: AbilityDescriptor.h
// type: c++ header
// desc: template class
// auth: is0urce

// template for generic skill
// _U - user, _T - target

#pragma once

#include "ability.h"
#include "skill.h"

#include <functional>
#include <stdexcept>

namespace px
{
	struct Point;

	template <typename _U, typename _T>
	class delegate_ability : public ability <_T>, public rl::skill
	{
	public:
		typedef std::function<void(_U, _T)> target_fn;
		typedef std::function<bool(_U, _T)> target_check_fn;
		typedef std::function<void(_U, const point&)> ground_fn;
		typedef std::function<bool(_U, const point&)> ground_check_fn;
		typedef unsigned int range_component;
		typedef std::pair<range_component, range_component> range_t;

	protected:
		bool m_targeted;
		target_fn m_target;
		target_check_fn m_target_check;
		ground_fn m_ground;
		ground_check_fn m_ground_check;
		_U m_user;

	public:
		delegate_ability(_U user, target_fn tf, target_check_fn tfc) :
			m_user(user),
			m_targeted(true),
			m_target(tf), m_target_check(tfc)
		{
			if (!tf) throw std::logic_error("px::targeted_ability::targeted_ability() - tf is null");
			if (!tfc) throw std::logic_error("px::targeted_ability::targeted_ability() - tfc is null");
		}
		delegate_ability(_U user, ground_fn gf, ground_check_fn gfc) :
			m_user(user),
			m_targeted(false),
			m_ground(gf), m_ground_check(gfc)
		{
			if (!gf) throw std::logic_error("px::targeted_ability::targeted_ability() - gf is null");
			if (!gfc) throw std::logic_error("px::targeted_ability::targeted_ability() - gfc is null");
		}
		virtual ~delegate_ability() {}

	protected:
		virtual bool targeted_ability() const override { return m_targeted; };
		virtual void use_ability(_T target) override
		{
			if (!m_target) throw std::logic_error("px::targeted_ability::use_abitity(..) - m_target is null");

			m_target(m_user, target);
		}
		virtual void use_ability(const point &target)
		{
			if (!m_ground) throw std::logic_error("px::targeted_ability::use_abitity(..) - m_ground is null");

			m_ground(m_user, target);
		}
		virtual bool useable_ability(_T target) const override
		{
			if (!m_target_check) throw std::logic_error("px::targeted_ability::useable_ability(..) - m_target_check is null");

			return m_target_check(m_user, target);
		}
		virtual bool useable_ability(const point &target) const override
		{
			if (!m_ground_check) throw std::logic_error("px::targeted_ability::useable_ability(..) - m_ground_check is null");

			return m_ground_check(m_user, target);
		}

	public:
		_U user()
		{
			return m_user;
		}
	};
}