// name: actor.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_GENERATOR_H
#define PX_RL_GENERATOR_H

#include <px/unit.h>
#include <px/inventory.h>
#include <px/item.h>

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
			virtual void use_unit(user_t user) override {}
			virtual bool useable_unit(user_t user) const override { return true; }
		};
	}
}

#endif