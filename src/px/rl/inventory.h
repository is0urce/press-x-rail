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
			typedef inventory<_I> container_t;
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
			void enumerate_items(std::function<void(_I)> fn) const
			{
				std::for_each(m_items.begin(), m_items.end(), fn);
			}
			void enumerate_items(std::function<void(_I)> fn, std::function<bool(_I)> filter_fn) const
			{
				for (auto &i : m_items)
				{
					if (filter_fn(i))
					{
						fn(i);
					}
				}
			}
			unsigned int item_count() const
			{
				return m_items.size();
			}
			void add_item(_I item, bool silent)
			{
				m_items.push_back(item); acquire(item, silent);
			}
			void add_item(_I item)
			{ 
				add_item(item, false);
			}
			bool remove_item(_I item, bool silent)
			{
				unaquire(item, silent);
				m_items.remove(item);
				return true;
			}
			bool remove_item(_I item)
			{
				return remove_item(item, false);
			}
			void clear(bool silent)
			{
					for (auto &i : m_items)
					{
						unaquire(i, silent);
					}
					m_items.clear();
			}
			void clear()
			{
				clear(false);
			}
			bool move(_I item, container_t &destination, bool silent)
			{
				bool result = remove_item(item);
				if (result)
				{
					destination.add_item(item, silent);
				}
				return result;
			}
			bool move(_I item, container_t &destination)
			{
				return move(item, destination, false);
			}
			void move_all(container_t &destination)
			{
				move_all(destination, false);
			}
			void move_all(container_t &destination, bool silent)
			{
				for (auto &i : m_items)
				{
					destination.add_item(i, silent);
				}
				clear(silent);
			}
		};
	}
}

#endif