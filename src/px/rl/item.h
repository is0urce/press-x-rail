// name: item.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_ITEM_H
#define PX_RL_ITEM_H

#include <px/rl/unit.h>

namespace px
{
	namespace rl
	{
		class person;

		class item : public unit
		{
		public:
			typedef std::shared_ptr<person> activator_ptr;
			typedef double weight_t;

		private:
			bool m_stackable;
			weight_t m_weight;

		public:
			item();
			virtual ~item();

		public:
			static sign_t signature();

		protected:
			virtual void activate_item(activator_ptr activator);
			virtual sign_t sign_unit() const override;
			virtual void serialize(o_node node, const serializer& s) const override;
			virtual void deserialize(i_node node, const serializer& s) override;

		public:
			void activate(activator_ptr activator);
			bool stackable() const;
			void stackable(bool is_stackable);
			weight_t weight() const;
			void weight(weight_t weight_value);
		};
	}
}

#endif