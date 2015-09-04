// name: character.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "bar.h"
#include "statistics.h"
#include "attribute.h"
#include "status.h"

#include "ability.h"

#include <stdexcept>

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
			typedef ability<std::shared_ptr<unit>>  ability_t;
			typedef std::shared_ptr<ability_t> ability_ptr;

		protected:
			resource_t m_hp;
			resource_t m_mp;
			stats_t m_base;
			stats_t m_computed; // + modifiers
			std::vector<ability_ptr> m_skills;
			std::list<status<character>> m_affect;

			// ctor & dtor
		public:
			character() : m_base(max_attribute), m_computed(max_attribute) {}
			//character(const character&) = delete;
			virtual ~character() {}

		public:
			resource_t& health() { return m_hp; }
			const resource_t& health() const { return m_hp; }
			resource_t& energy() { return m_mp; }
			const resource_t& energy() const { return m_mp; }

			bool dead() const { return m_hp.empty(); }

			stat_value statistic(attribute a) { return m_base[a]; }
			ability_ptr skill(unsigned int slot)
			{
				return m_skills.at(slot);
			}
		};
	}
}