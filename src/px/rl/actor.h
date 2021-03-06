// name: actor.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_ACTOR_H
#define PX_RL_ACTOR_H

#include <px/rl/unit.h>
#include <px/rl/inventory.h>
#include <px/rl/item.h>

#include <memory>
#include <vector>

namespace px
{
	namespace rl
	{
		class actor : 
			public unit,
			public inventory<std::shared_ptr<item>>
		{
			// ctor & dtor
		public:
			actor() {}
			virtual ~actor() {}

			// vitrual
		protected:
			virtual void use_unit(environment&, user_t user) override {}
			virtual bool useable_unit(const environment&, user_t user) const override { return true; }
		};
	}
}

#endif