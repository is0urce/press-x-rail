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
			typedef unsigned int time_t;
			typedef _T& _R;
			typedef std::function<void(_R, time_t duration)> effect_fn;

		private:
			unsigned int m_duration;
			effect_fn m_apply, m_expire, m_tick;
			std::string m_name;

		public:
			status(time_t duration, effect_fn tick, effect_fn apply, effect_fn expire)
				: m_duration(duration), m_tick(tick), m_apply(apply), m_expire(expire) {}
			status(time_t duration, effect_fn tick)
				: m_duration(duration), m_tick(tick) {}
			~status() {}

		public:
			time_t duration() const { return m_duration; }
			bool expired() { return m_duration == 0; }

			void on_apply(_R target) { if (m_apply) m_apply(target, m_duration); }
			void on_expire(_R target) { if (m_expire) m_expire(target, m_duration); }
			void on_tick(_R target) { if (!expired && m_tick) m_tick(target, m_duration); --_duration; }
			void on_tick(_R target, time_t delta) { for (unsigned int i = 0; i < delta; ++i) on_tick(actor); }

			std::string display() const { std::stringstream ss; ss << m_name << " (" << m_duration << ")"; return ss.str(); }
		};
	}
}