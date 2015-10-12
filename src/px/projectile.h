// name: projectile.h
// type: c++ header
// desc: struct definition
// auth: is0urce

#ifndef PX_PROJECTILE_H
#define PX_PROJECTILE_H

#include <px/appearance.h>
#include <px/color.h>
#include <px/color.h>
#include <px/vector.h>

#include <functional>
#include <stdexcept>

namespace px
{
	struct projectile
	{
	public:
		typedef appearance<unsigned int> appearance_t;
		typedef light light_t;
		typedef double timespan_t;
		typedef std::function<vector(timespan_t)> position_fn;
		typedef std::function<double(timespan_t)> rotation_fn;
		typedef std::function<double(timespan_t)> scale_fn;

	private:
		appearance_t m_appearance;
		light_t m_light;
		position_fn m_position;
		rotation_fn m_rotation;
		scale_fn m_scale;

	private:
		static rotation_fn::result_type rotation_none(timespan_t t)
		{
			return 0.0;
		}
		static scale_fn::result_type scale_none(timespan_t t)
		{
			return 1.0;
		}

	public:
		projectile(appearance_t appearance, light_t light, position_fn pos_fn, rotation_fn rotation_fn, scale_fn scale_fn) : 
			m_appearance(appearance),
			m_light(light),
			m_position(pos_fn),
			m_rotation(rotation_fn ? rotation_fn : &projectile::rotation_none),
			m_scale(scale_fn ? scale_fn : &projectile::scale_none)
		{
			if (!pos_fn) throw std::logic_error("px::projectile::ctor() pos_fn is null");
		}
		projectile(appearance_t appearance, light_t light, position_fn pos_fn, double radians_angle, double scale_value) :
			m_appearance(appearance),
			m_light(light),
			m_position(pos_fn),
			m_rotation([radians_angle](timespan_t) { return radians_angle; }),
			m_scale([scale_value](timespan_t t) { return scale_value; })
		{
			if (!pos_fn) throw std::logic_error("px::projectile::ctor() pos_fn is null");
		}

		const appearance_t& appearance() const
		{
			return m_appearance;
		}
		const light_t& light() const
		{
			return m_light;
		}
		
		// magnitude of the effect
		double magnitude(timespan_t time) const
		{
			//return time;
			auto time2 = time - 0.5;
			time2 = time2 * time2;
			time2 = 0.25 - time2;
			return (time2 > 0) ? std::sqrt(time2) : 0.0;
		}
		position_fn::result_type position(timespan_t time) const
		{
			return m_position(time);
		}
		rotation_fn::result_type rotation(timespan_t time) const
		{
			return m_rotation(time);
		}
		scale_fn::result_type scale(timespan_t time) const
		{
			return m_scale(time);
		}
	};
}

#endif