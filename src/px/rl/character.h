// name: character.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_CHARACTER_H
#define PX_RL_CHARACTER_H

#include <px/bar.h>
#include <px/rl/statistics.h>
#include <px/rl/status.h>
#include <px/rl/attribute.h>

#include <memory>
#include <list>

namespace px
{
	class writer_node;
	class reader_node;
	namespace rl
	{
		class character
		{
		public:
			typedef std::shared_ptr<writer_node> o_node;
			typedef const reader_node& i_node;
			typedef int stat_value;
			typedef bar<stat_value> resource_t;
			//typedef enhancement<attribute, stat_value> enhancement_t;
			typedef statistics<stat_value> stats_t;

		protected:
			resource_t m_hp;
			resource_t m_mp;
			stats_t m_base;
			stats_t m_computed; // + modifiers

			// ctor & dtor
		public:
			character();
			virtual ~character();

		protected:
			void store(o_node node) const;
			void restore(i_node node);

		public:
			resource_t& health();
			const resource_t& health() const;
			resource_t& energy();
			const resource_t& energy() const;

			bool dead() const;

			stat_value statistic(attribute a);
		};
	}
}

#endif