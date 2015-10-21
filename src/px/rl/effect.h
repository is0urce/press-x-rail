// name: effect.h
// type: c++ header
// desc: declaration of interface
// auth: is0urce

// forms visitor pattern with conjunction with unit.apply virtual method

#ifndef PX_RL_EFFECT_H
#define PX_RL_EFFECT_H

#include <px/rl/visitor.h>
#include <px/rl/person.h> 

namespace px
{
	namespace rl
	{
		class effect : public visitor
		{
		public:
			typedef person::status_t status_t;

		private:
			status_t m_status;

			// ctor & dtor
		public:
			effect(status_t affect) : m_status(affect)
			{
			}
			virtual ~effect()
			{
			}

			// virtual methods
		protected:
			virtual void expose_person(person &mobile) override
			{
				mobile.add_status(m_status);
			}
		};
	}
}

#endif