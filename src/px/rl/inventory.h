// name: inventory.h
// type: c++ header
// desc: class
// auth: is0uce

#ifndef PX_RL_INVENTORY_H
#define PX_RL_INVENTORY_H

#include <list>
#include <functional>
#include <algorithm>

namespace px
{
	namespace rl
	{
		template <typename _I>
		class inventory
		{
		public:
			typedef _I item_t;

		private:
			std::list<_I> m_items;

		public:
			inventory() {}
			virtual ~inventory() {}

		protected:
			virtual void acquire(_I item, bool silent) {}
			virtual void unaquire(_I item, bool silent) {}

		public:
			void enumerate_items(std::function<void(_I)> fn) const { std::for_each(m_items.begin(), m_items.end(), fn); }
			unsigned int item_count() const { return m_items.size(); }
			void add_item(_I item, bool silent) { m_items.push_back(item); acquire(item, silent); }
			void add_item(_I item) { add_item(item, false); }
			void remove_item(_I item, bool silent) { m_items.remove(item); unaquire(item, silent); }
			void remove_item(_I item) { remove_item(item, false); }
		};
	}
}

#endif