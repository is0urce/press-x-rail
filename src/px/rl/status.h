// name: status.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_STATUS_H
#define PX_RL_STATUS_H

#include <px/rl/entity.h>

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
			typedef _T target_t;
			typedef std::function<void(_T&, status&)> effect_fn;

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
			void duration(time_t duration_value)
			{
				m_duration = duration_value;
			}
			bool expired() const
			{
				return m_duration == 0;
			}
			void register_tick(effect_fn fn)
			{
				m_tick = fn;
			}
			void register_apply(effect_fn fn)
			{
				m_apply = fn;
			}
			void register_expire(effect_fn fn)
			{
				m_expire = fn;
			}


			void on_apply(target_t& target)
			{
				if (m_apply)
				{
					m_apply(target, *this);
				}
			}
			void on_expire(target_t& target)
			{
				if (m_expire)
				{
					m_expire(target, *this);
				}
			}
			void on_tick(target_t& target)
			{
				if (!expired())
				{
					if (m_tick)
					{
						m_tick(target, *this);
					}
					--m_duration;
				}
			}
			void on_tick(target_t& target, timer_t delta)
			{
				for (unsigned int i = 0; i < delta; ++i)
				{
					on_tick(target);
				}
			}
		};
	}
}

#endif