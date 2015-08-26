// name: deposit.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "unit.h"
#include "item.h"
#include "actor.h"

namespace px
{
	namespace rl
	{
		class deposit : public unit
		{
		private:
			bool m_depleted;
			std::shared_ptr<item> m_item;

			// ctor & dtor
		public:
			deposit() {}
			virtual ~deposit() {}

			// vitrual
		protected:
			virtual void use_unit(user_t user) override { user->add_item(m_item); }
			virtual bool useable_unit() const override { return !depleted(); }

		public:
			bool depleted() const { return m_depleted; }
		};
	}
}