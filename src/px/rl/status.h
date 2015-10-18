// name: status.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "entity.h"

#include <functional>

namespace px
{
	namespace rl
	{
		template <typename _T>
		class status : public entity
		{
		public:
			typedef unsigned int timer_t;
			typedef _T& _R;
			typedef std::function<void(_R, timer_t duration)> effect_fn;

		private:
			timer_t m_duration;
			effect_fn m_apply, m_expire, m_tick;

		public:
			status(timer_t duration)
				: m_duration(duration) {}
			status(timer_t duration, effect_fn tick, effect_fn apply, effect_fn expire)
				: m_duration(duration), m_tick(tick), m_apply(apply), m_expire(expire) {}
			status(timer_t duration, effect_fn tick)
				: m_duration(duration), m_tick(tick) {}
			~status() {}

		public:
			time_t duration() const
			{
				return m_duration;
			}
			bool expired() const
			{
				return m_duration == 0;
			}

			void on_apply(_R target) const
			{
				if (m_apply)
				{
					m_apply(target, m_duration);
				}
			}
			void on_expire(_R target) const
			{
				if (m_expire)
				{
					m_expire(target, m_duration);
				}
			}
			void on_tick(_R target)
			{
				if (!expired())
				{
					if (m_tick)
					{
						m_tick(target, m_duration);
					}
					--m_duration;
				}
			}
			void on_tick(_R target, timer_t delta)
			{
				for (unsigned int i = 0; i < delta; ++i)
				{
					on_tick(target);
				}
			}
		};
	}
}