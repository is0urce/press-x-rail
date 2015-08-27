// name: unit.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "entity.h"
#include "point.h"

#include <memory>

namespace px
{
	namespace rl
	{
		class actor;
		class unit : public entity
		{
		public:
			typedef std::shared_ptr<actor> user_t;

		private:	
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
			virtual bool useable_unit(user_t user) const;
			virtual void use_unit(user_t user);

		public:
			bool traversable() const;
			bool transparent() const;
			bool useable(user_t user) const;
			void use(user_t user);

			// position
			const point& position() const;
			const point& previous_position() const;
			void position(const point& position);
			void position(const point& position, const point& previous);
			void store_position();

			void destroy();
			bool destroying() const;
		};
	}
}