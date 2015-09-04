// name: skill.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "entity.h"

namespace px
{
	namespace rl
	{
		class skill : public entity
		{
		public:
			typedef int resource_t;
			typedef unsigned int timespan_t;
			typedef unsigned int range_component;
			typedef std::pair<range_component, range_component> range_t;

		private:
			resource_t m_cost;
			timespan_t m_cooldown;
			timespan_t m_cooldown_timer;
			range_t m_range;

		public:
			skill() : m_cost(0), m_cooldown(0), m_cooldown_timer(0), m_range({ 0, 10000 }) {}
			virtual ~skill() {}

		public:
			resource_t cost() const { return m_cost; }
			void cost(resource_t cost_value) { m_cost = cost_value; }

			range_t range() const { return m_range; }
			void range(range_t ranges) { m_range = ranges; }
			bool in_range(range_component test) { return test >= m_range.first && test <= m_range.second; }

			// cooldown
			timespan_t cooldown() const { return m_cooldown; }
			void cooldown(timespan_t cooldown_time) { m_cooldown = cooldown_time; }
			timespan_t cooldown_remain() const { return m_cooldown_timer; }
			bool ready() const { return m_cooldown_timer == 0; }
			void refresh(timespan_t time) { m_cooldown_timer = time >= m_cooldown_timer ? 0 : m_cooldown_timer - time; }
			void refresh() { refresh(1); }
			void start_cooldown() { m_cooldown_timer = m_cooldown; }
			void start_cooldown(timespan_t cooldown_time) { m_cooldown_timer = cooldown_time; }
			void end_cooldown() { m_cooldown_timer = 0; }
		};
	}
}