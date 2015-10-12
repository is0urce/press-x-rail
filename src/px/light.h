// name: light.h
// type: c++ header
// desc: class with implementation
// auth: is0urce

#ifndef PX_LIGHT_H
#define PX_LIGHT_H

#include <px/color.h>

namespace px
{
	class light
	{
	public:
		typedef color color_t;

	private:
		color_t m_color;
		bool m_enabled;
		bool m_shadowcast;

	public:
		light() 
			: m_enabled(false), m_shadowcast(false) {}
		light(color_t color_value)
			: m_enabled(true), m_shadowcast(false), m_color(color_value) {}
		light(color_t color_value, bool is_enabled, bool casts_shadows)
			: m_enabled(is_enabled), m_shadowcast(casts_shadows), m_color(color_value) {}

		void enable() { m_enabled = false; }
		void disable() { m_enabled = false; }
		void toggle() { m_enabled = !m_enabled; }

		color_t color() const { return m_color; }
		bool shadowcasting() const { return m_shadowcast; }
		bool enabled() const { return m_enabled; }
	};
}

#endif