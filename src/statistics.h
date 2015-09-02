// name: Statistics.h
// type: c++
// desc: class header
// auth: is0urce

#pragma once

#include "enhancement.h"

#include <vector>

namespace px
{
	namespace rl
	{
		template <typename _T>
		class statistics
		{
		private:
			std::vector<_T> m_values;

		public:
			statistics(unsigned int size) : m_values(size, 0) {}
			~statistics() {}

		public:
			void clear()
			{
				std::fill(m_values.begin(), m_values.end(), 0);
			}

			template <typename _A>
			void enhance(_A stat, _T modify) { m_values.at((unsigned int)stat) += modify; }
			template <typename _A>
			void enhance(const enhancement<_A, _T> &enchant) { enhance(enchant.attribute(), enchant.value()); }
			template <typename _L>
			void enhance(const _L &list) {
				std::for_each(list.begin(), list.end(), [&](const _L::value_type &e) { enhance(e); });
			}

			template <typename _A>
			_T& operator[] (_A n) { return m_values.at((unsigned int)n); }
			template <typename _A>

			const _T& operator[] (_A n) const { return m_values.at((unsigned int)n); }
		};
	}
}