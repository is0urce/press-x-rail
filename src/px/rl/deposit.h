// name: deposit.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_DEPOSIT_H
#define PX_RL_DEPOSIT_H

#include <px/rl/unit.h>

namespace px
{
	namespace rl
	{
		class item;
		class deposit : public unit
		{
		public:
			typedef std::shared_ptr<item> resource_t;
		private:
			bool m_depleted;
			resource_t m_item;

			// ctor & dtor
		public:
			deposit();
			deposit(resource_t resource);
			virtual ~deposit();

		public:
			static sign_t signature();

			// vitrual
		protected:
			virtual bool useable_unit(const environment&, user_t user) const override;
			virtual void use_unit(environment&, user_t user) override;
			virtual sign_t sign_unit() const override;
			virtual void serialize(o_node node, const serializer &s) const override;
			virtual void deserialize(const i_node &node, const serializer &s) override;

		public:
			void renew(resource_t resource);
			bool depleted() const;
		};
	}
}

#endif