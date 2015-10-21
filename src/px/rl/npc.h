// name: npc.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_NPC_H
#define PX_RL_NPC_H

#include <px/rl/person.h>

namespace px
{
	namespace rl
	{
		class npc : public person
		{
		public:
			enum class ai_state : unsigned int;

		private:
			ai_state m_ai_state;

			// ctor & dtor
		public:
			npc();
			virtual ~npc();

		public:
			static sign_t signature();

		private:
			virtual sign_t sign_unit() const override;
			virtual void action_unit(environment &current) override;
		};
	}
}

#endif