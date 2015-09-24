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

		private:
			bool m_stackable;
		public:
			item() : m_stackable(false) {}
			virtual ~item() {}

		protected:
			virtual void activate_item(activator_ptr activator) {}

		public:
			void activate(activator_ptr activator) { activate_item(activator); }
			bool stackable() const { return m_stackable; }
			void stackable(bool is_stackable) { m_stackable = is_stackable; }
		};
	}
}

#endif