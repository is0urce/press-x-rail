// name: unit.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "point.h"

#include <memory>

namespace px
{
	namespace rl
	{
		class actor;
		class unit
		{
		public:
			typedef unsigned int appearance_t;
			typedef std::string name_t;
			typedef unsigned int faction_t;
			typedef std::shared_ptr<actor> user_t;

		private:	
			name_t m_name;
			appearance_t m_appearance;
			faction_t m_faction;

			point m_position;
			point m_prev_position;
			bool m_remove;

			// ctor & dtor
		public:
			unit();
			unit(const point& position);
			virtual ~unit();
		private:
			unit(const unit&);

			// vitrual
		protected:
			virtual bool traversable_unit() const;
			virtual bool transparent_unit() const;
			virtual bool useable_unit() const;
			virtual void use_unit(user_t user);

		public:
			bool traversable() const;
			bool transparent() const;
			bool useable() const;
			void use(user_t user);

			// position
			const point& position() const;
			const point& previous_position() const;
			void position(const point& position);
			void position(const point& position, const point& previous);
			void position_remember();

			// appearance
			void appearance(appearance_t img);
			appearance_t appearance() const;
			void name(name_t str);
			name_t name() const;

			void faction(faction_t id);
			int reputation(faction_t id) const;
			int reputation(const unit &unit) const;

			void destroy();
			bool destroying() const;
		};
	}
}