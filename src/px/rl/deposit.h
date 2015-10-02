// name: deposit.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_DEPOSIT_H
#define PX_RL_DEPOSIT_H

#include <px/rl/unit.h>
#include <px/rl/item.h>
#include <px/rl/actor.h>

namespace px
{
	namespace rl
	{
		class deposit : public unit
		{
		public:
			typedef std::shared_ptr<item> resource_ptr;
		private:
			bool m_depleted;
			resource_ptr m_item;

			// ctor & dtor
		public:
			deposit(resource_ptr resource) : m_depleted(false), m_item(resource) {}
			virtual ~deposit() {}

			// vitrual
		protected:
			virtual bool useable_unit(const environment&, user_t user) const override { return !depleted(); }
			virtual void use_unit(environment&, user_t user) override { user->add_item(m_item); m_depleted = true; m_item.reset(); m_appearance.color.shift_brightness(0.5); }

		public:
			bool depleted() const { return m_depleted; }
		};
	}
}

#endif