// name: character.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "bar.h"
#include "statistics.h"
#include "attribute.h"
#include "status.h"

namespace px
{
	namespace rl
	{
		class character
		{
		private:
			static const unsigned int max_attribute = (unsigned int)attribute::max_attribute;

		public:
			typedef int stat_value;
			typedef bar<stat_value> resource_t;
			//typedef enhancement<attribute, stat_value> enhancement_t;
			typedef statistics<stat_value> stats_t;
			typedef int ability_t;

		private:
			resource_t m_hp;
			resource_t m_mp;
			stats_t m_base;
			stats_t m_computed; // + modifiers
			std::vector<ability_t> m_skills;
			std::list<status<character>> m_affect;

			// ctor & dtor
		public:
			character() : m_base(max_attribute), m_computed(max_attribute) {}
			virtual ~character() {}

		public:
			resource_t& health() { return m_hp; }
			const resource_t& health() const { return m_hp; }
			resource_t& energy() { return m_mp; }
			const resource_t& energy() const { return m_mp; }

			bool dead() const { return m_hp.empty(); }

			stat_value statistic(attribute a) { return m_base[a]; }
			ability_t& skill(unsigned int slot) { return m_skills.at(slot); }
		};
	}
}