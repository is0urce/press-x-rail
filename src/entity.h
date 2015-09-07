// name: entity.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "appearance.h"

#include "reader.h"
#include "writer.h"

#include <string>

namespace px
{
	namespace rl
	{
		class entity
		{
		public:
			typedef appearance<unsigned int> appearance_t;
			typedef std::string name_t;
			typedef std::string tag_t;
			typedef unsigned int faction_t;

		protected:
			name_t m_name;
			tag_t m_tag;
			appearance_t m_appearance;
			faction_t m_faction;

			// ctor & dtor
		public:
			entity();
			virtual ~entity();

		protected:
			void save(writer::node_ptr node) const;
			void load(const reader::node &node);

		public:
			// attribute accessors
			void appearance(appearance_t img);
			appearance_t appearance() const;
			void name(name_t str);
			name_t name() const;
			void tag(tag_t str);
			tag_t tag() const;

			// faction
			void faction(faction_t handle);
			int reputation(faction_t handle) const;
			int reputation(const entity &member) const;
		};
	}
}