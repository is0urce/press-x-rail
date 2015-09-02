// name: npc.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "person.h"

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
		};
	}
}