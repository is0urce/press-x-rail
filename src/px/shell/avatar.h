// name: avater.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_SHELL_AVATAR_H
#define PX_SHELL_AVATAR_H

// model view of unit

#include <px/point.h>

namespace px
{
	namespace shell
	{
		template <typename _A>
		class avatar
		{
		private:
			_A m_appearance;
			point m_current;
			point m_previous;

		public:
			avatar(_A appearance, point position, point previous) : m_appearance(appearance), m_current(position), m_previous(previous)
			{
			}
			~avatar() {}

		public:
			const point& position() const { return m_current; };
			const point& position_previous() const { return m_previous; };
			const _A& appearance() const { return m_appearance; };
		};
	}
}

#endif