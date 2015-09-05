// name: delegate_action.h
// type: c++ header
// desc: template class
// auth: is0urce

// template for skill activation functional
// _U - user, _T - target

#pragma once

#include "skill.h"

#include <functional>
#include <stdexcept>

namespace px
{
	struct Point;

	template <typename _U, typename _T>
	class delegate_action : public rl::skill, public rl::entity
	{
	public:
		typedef std::function<void(_U, _T)> target_fn;
		typedef std::function<bool(_U, _T)> target_check_fn;
		typedef std::function<void(_U, const point&)> ground_fn;
		typedef std::function<bool(_U, const point&)> ground_check_fn;

	private:
		bool m_targeted;
		target_fn m_target;
		target_check_fn m_target_check;
		ground_fn m_ground;
		ground_check_fn m_ground_check;

	public:
		delegate_action(target_fn tf, target_check_fn tfc) :
			m_targeted(true),
			m_target(tf), m_target_check(tfc)
		{
			if (!tf) throw std::logic_error("px::targeted_ability::targeted_ability() - tf is null");
			if (!tfc) throw std::logic_error("px::targeted_ability::targeted_ability() - tfc is null");
		}
		delegate_action(ground_fn gf, ground_check_fn gfc) :
			m_targeted(false),
			m_ground(gf), m_ground_check(gfc)
		{
			if (!gf) throw std::logic_error("px::targeted_ability::targeted_ability() - gf is null");
			if (!gfc) throw std::logic_error("px::targeted_ability::targeted_ability() - gfc is null");
		}
		virtual ~delegate_action() {}

	public:
		bool targeted() const { return m_targeted; }
		void use(_U user, _T target)
		{
			if (!m_target) throw std::logic_error("px::targeted_ability::use_abitity(..) - m_target is null");

			m_target(user, target);
		}
		void use(_U user, const point &target)
		{
			if (!m_ground) throw std::logic_error("px::targeted_ability::use_abitity(..) - m_ground is null");

			m_ground(user, target);
		}
		bool useable(_U user, _T target) const
		{
			if (!m_target_check) throw std::logic_error("px::targeted_ability::useable_ability(..) - m_target_check is null");

			return m_target_check(user, target);
		}
		bool useable(_U user, const point &target) const
		{
			if (!m_ground_check) throw std::logic_error("px::targeted_ability::useable_ability(..) - m_ground_check is null");

			return m_ground_check(user, target);
		}
	};
}