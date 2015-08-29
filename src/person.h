// name: actor.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "actor.h"
#include "bar.h"

#include <memory>

namespace px
{
	namespace rl
	{
		class person : public actor
		{
		public:
			typedef bar<int> resource_t;
		private:
			resource_t m_hp;
			resource_t m_mp;

			// ctor & dtor
		public:
			person() {}
			virtual ~person() {}

		public:
			resource_t& health() { return m_hp; }
			const resource_t& health() const { return m_hp; }
			resource_t& energy() { return m_mp; }
			const resource_t& energy() const { return m_mp; }

			bool dead() const { return m_hp.empty(); }
		};
	}
}