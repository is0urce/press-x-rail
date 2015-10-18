// name: entity.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_ENTITY_H
#define PX_RL_ENTITY_H

#include <px/appearance.h>
#include <px/light.h>

#include <memory>
#include <string>

namespace px
{
	class writer_node;
	class reader_node;
	namespace rl
	{
		class entity
		{
		public:
			typedef appearance<unsigned int> appearance_t;
			typedef std::string name_t;
			typedef std::string tag_t;
			typedef unsigned int fraction_t;
			typedef light light_t;
			typedef std::shared_ptr<writer_node> o_node;
			typedef const reader_node& i_node;

		protected:
			name_t m_name;
			tag_t m_tag;
			appearance_t m_appearance;
			fraction_t m_fraction;
			light_t m_light;

			// ctor & dtor
		public:
			entity();
			virtual ~entity();

		protected:
			void store(o_node node) const;
			void restore(i_node node);

		public:
			// attribute accessors
			void appearance(appearance_t img);
			appearance_t appearance() const;
			void name(name_t str);
			name_t name() const;
			void tag(tag_t str);
			tag_t tag() const;
			const light_t& light() const;
			light_t& light();
			void light(light_t light_source);

			// faction
			void fraction(fraction_t handle);
			int reputation(fraction_t handle) const;
			int reputation(const entity &member) const;
		};
	}
}

#endif