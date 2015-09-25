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
			item() : m_stackable(false), m_weight{} {}
			virtual ~item() {}

		protected:
			virtual void activate_item(activator_ptr activator) {}

		public:
			void activate(activator_ptr activator) { activate_item(activator); }
			bool stackable() const { return m_stackable; }
			void stackable(bool is_stackable) { m_stackable = is_stackable; }
			weight_t weight() const { return m_weight; }
			void weight(weight_t weight_value) { m_weight = weight_value; }
		};
	}
}

#endif