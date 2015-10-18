// name: character.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "character.h"

#include <px/reader_node.hpp>
#include <px/writer_node.hpp>

namespace px
{
	namespace rl
	{
		namespace
		{
			const unsigned int max_attribute = (unsigned int)attribute::max_attribute;
		}

		character::character() : m_base(max_attribute), m_computed(max_attribute)
		{
		}
		character::~character()
		{
		}

		void character::store(o_node node) const
		{
			node->write("hp", m_hp);
			node->write("mp", m_mp);
			node->write("stats", (char*)&m_base[0], sizeof(&m_base[0]) * m_base.size());
		}
		void character::restore(i_node node)
		{
			node["hp"] >> m_hp;
			node["mp"] >> m_mp;
			node["stats"].read((char*)&m_base[0], sizeof(&m_base[0]) * m_base.size());
		}

		character::resource_t& character::health()
		{
			return m_hp;
		}
		const character::resource_t& character::health() const
		{
			return m_hp;
		}
		character::resource_t& character::energy()
		{ 
			return m_mp;
		}
		const character::resource_t& character::energy() const
		{
			return m_mp;
		}

		void character::add_status(status_t affect)
		{
			m_affect.push_back(affect);
			affect.on_apply(*this);
		}
		void character::tick(status_t::timer_t span)
		{
			for (auto &s : m_affect)
			{
				s.on_tick(*this, span);
				if (s.expired())
				{
					s.on_expire(*this);
				}
			}
			m_affect.erase(std::remove_if(m_affect.begin(), m_affect.end(), [](const status_t &s)
			{
				return s.expired();
			}), m_affect.end());
		}
		void character::enumerate_affects(std::function<void(const status_t&)> enum_fn) const
		{
			std::for_each(m_affect.begin(), m_affect.end(), enum_fn);
		}

		bool character::dead() const
		{
			return m_hp.empty();
		}
	}
}