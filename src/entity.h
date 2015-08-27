// name: entity.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include <string>

namespace px
{
	namespace rl
	{
		class entity
		{
		public:
			typedef unsigned int appearance_t;
			typedef std::string name_t;
			typedef unsigned int faction_t;

		private:
			name_t m_name;
			appearance_t m_appearance;
			faction_t m_faction;

			// ctor & dtor
		public:
			entity();
			virtual ~entity();
		private:
			entity(const entity&);

		public:
			// attribute accessors
			void appearance(appearance_t img);
			appearance_t appearance() const;
			void name(name_t str);
			name_t name() const;

			// faction
			void faction(faction_t handle);
			int reputation(faction_t handle) const;
			int reputation(const entity &member) const;
		};
	}
}