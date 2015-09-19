// name: avater.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_SHELL_AVATAR_H
#define PX_SHELL_AVATAR_H

// model view of unit

#include <px/point.h>
#include <px/light.h>

namespace px
{
	namespace shell
	{
		template <typename _A>
		class avatar
		{
		public:
			typedef _A appearance_t;
			typedef light light_t;

		private:
			_A m_appearance;
			light_t m_light;
			point m_current;
			point m_previous;

		public:
			avatar(_A appearance, light_t light_source, point position, point previous)
				: m_appearance(appearance), m_current(position), m_previous(previous), m_light(light_source)
			{
			}
			~avatar() {}

		public:
			const point& position() const { return m_current; }
			const point& position_previous() const { return m_previous; }
			const _A& appearance() const { return m_appearance; }
			const light& light() const { return m_light; }
		};
	}
}

#endif