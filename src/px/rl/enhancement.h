// name: enhancement.h
// type: c++
// desc: class header
// auth: is0urce

// buffs and debuffs

#ifndef PX_RL_ENHANCEMENT_H
#define PX_RL_ENHANCEMENT_H

namespace px
{
	namespace rl
	{
		template <typename _I, typename _T>
		class enhancement
		{
		private:
			_I m_index;
			_T m_modify;

		public:
			enhancement(_I stat, _T mod) : m_index(stat), m_modify(mod) {}
			~enhancement() {}

		public:
			_I attribute() const { return m_index; }
			_T value() const { return m_modify; }
		};
	}
}

#endif