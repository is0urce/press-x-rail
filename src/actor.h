// name: actor.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "unit.h"
#include "inventory.h"
#include "item.h"

#include <memory>

namespace px
{
	namespace rl
	{
		class actor : public unit, inventory<std::shared_ptr<item>>
		{
		private:

			// ctor & dtor
		public:
			actor() {}
			virtual ~actor() {}

			// vitrual
		protected:
			virtual void use_unit(user_t user) override {}
		};
	}
}