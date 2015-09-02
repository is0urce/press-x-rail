// name: person.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "actor.h"
#include "character.h"

#include "effect.h"

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

		protected:
			virtual void apply_effect(effect &e) override { e.apply(*this); }
		};
	}
}