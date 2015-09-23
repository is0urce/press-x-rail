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
		class Actor;

		class item : public unit
		{
		private:
			bool m_stackable;
		public:
			item() : m_stackable(false) {}
			virtual ~item() {}

		public:
			bool stackable() const { return m_stackable; }
			void stackable(bool is_stackable) { m_stackable = is_stackable; }
		};
	}
}

#endif