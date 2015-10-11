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
			// ctor & dtor
		public:
			npc() {}
			virtual ~npc() {}

		public:
			static sign_t signature() { return "npc"; }
		};
	}
}

#endif