// name: inventory.h
// type: c++ header
// desc: class
// auth: is0uce

#pragma once

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
		private:
			std::list<_I> m_items;

		public:
			void enumerate_items(std::function<void(_I)> fn) { std::for_each(m_items.begin(), m_items.end(), fn); }
			void add_item(_I item) { m_items.push_back(item); }
			void remove_item(_I item) { m_items.remove(item); }
			unsigned int count(_I item) { return m_items.size(); }
		};
	}
}