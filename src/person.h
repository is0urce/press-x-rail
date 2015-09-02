// name: person.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "actor.h"
#include "character.h"

namespace px
{
	namespace rl
	{
		class person : public actor, public character
		{
			// ctor & dtor
		public:
			person() {}
			virtual ~person() {}
		};
	}
}