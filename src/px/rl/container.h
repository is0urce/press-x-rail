// name: container.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_CONTAINER_H
#define PX_RL_CONTAINER_H

#include <px/rl/unit.h>
#include <px/rl/item.h>
#include <px/rl/inventory.h>

namespace px
{
	namespace rl
	{
		class container : public unit, public inventory<std::shared_ptr<item>>
		{
			// ctor & dtor
		public:
			container();
			virtual ~container();

			// vitrual
		protected:
			virtual void use_unit(environment&, user_t user) override;
			virtual bool useable_unit(const environment&, user_t user) const override;
		};
	}
}

#endif