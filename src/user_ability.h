// name: user_ability.h
// type: c++ header
// desc: template class
// auth: is0urce

// template for generic skill
// _U - user, _T - target

#pragma once

#include "ability.h"
#include "delegate_action.h"

#include <memory>
#include <functional>
#include <stdexcept>

namespace px
{
	template <typename _U, typename _T>
	class user_ability : public ability<_T>
	{
	public:
		typedef delegate_action<_U, _T> delegate_t;
		typedef std::shared_ptr<delegate_t> delegate_ptr;

	private:
		delegate_ptr m_delegate;
		_U m_user;

	public:
		user_ability(_U user, delegate_ptr delegate_ability_ptr) : m_user(user), m_delegate(delegate_ability_ptr)
		{
			if (!delegate_ability_ptr) throw std::logic_error("px::user_ability::ctor() - delegate_ptr is null");
		}
		virtual ~user_ability() {}

	protected:
		virtual bool targeted_ability() const override { return m_delegate->targeted(); }
		virtual void use_ability(_T target) override
		{
			m_delegate->use(m_user, target);
		}
		virtual void use_ability(const point &target) override
		{
			m_delegate->use(m_user, target);
		}
		virtual bool useable_ability(_T target) const override
		{
			return m_delegate->useable(m_user, target);
		}
		virtual bool useable_ability(const point &target) const override
		{
			return m_delegate->useable(m_user, target);
		}

	public:
		_U user()
		{
			return m_user;
		}
	};
}