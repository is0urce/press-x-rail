// name: unit.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_UNIT_H
#define PX_RL_UNIT_H

#include <px/rl/entity.h>
#include <px/point.h>

#include "px/reader.h"
#include "px/writer.h"

#include <memory>

namespace px
{
	class game;
	namespace rl
	{
		class effect;
		class actor;
		class unit : public entity
		{
		public:
			typedef std::shared_ptr<actor> user_t; // std::shared_ptr<actor>
			typedef game environment;

		private:	
			point m_position;
			point m_prev_position;
			bool m_remove;
			bool m_invincible;

			// ctor & dtor
		public:
			unit();
			unit(const point& position);
			virtual ~unit();

			// static
		public:
			static std::string signature();

			// vitrual
		protected:
			virtual bool traversable_unit() const;
			virtual bool transparent_unit() const;
			virtual bool useable_unit(const environment &current_environment, user_t user) const;
			virtual void use_unit(environment &current_environment, user_t user);
			virtual void apply_effect(effect &e);
			virtual void serialize(writer::node_ptr node) const;
			virtual void deserialize(const reader::node &node);
			virtual std::string sign_unit() const;

		public:
			std::string sign() const;
			void save(writer::node_ptr node) const;
			void load(const reader::node &node);

			bool traversable() const;
			bool transparent() const;
			bool useable(const environment &current_environment, user_t user) const;
			void use(environment &current_environment, user_t user);
			bool invincible() const;
			void invincible(bool is_invincible);

			// position
			const point& position() const;
			const point& previous_position() const;
			void position(const point& position);
			void position(const point& position, const point& previous);
			void store_position();

			void apply(effect &e);

			void destroy();
			bool destroying() const;
		};
	}
}

#endif