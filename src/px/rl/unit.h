// name: unit.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_UNIT_H
#define PX_RL_UNIT_H

#include <px/rl/entity.h>
#include <px/point.h>

#include <memory>
#include <string>

namespace px
{
	class writer_node;
	class reader_node;
	class game;

	namespace rl
	{
		class actor;
		class serializer;
		class visitor;
		class unit : public entity
		{
		public:
			typedef std::string sign_t;
			typedef std::shared_ptr<writer_node> o_node;
			typedef const reader_node& i_node;

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
			static sign_t signature();

			// vitrual
		protected:
			virtual bool traversable_unit() const;
			virtual bool transparent_unit() const;
			virtual void action_unit(environment &current);
			virtual bool useable_unit(const environment &current_environment, user_t user) const;
			virtual void use_unit(environment &current_environment, user_t user);
			virtual void accept_visitor(visitor&);
			virtual void serialize(o_node node, const serializer&) const;
			virtual void deserialize(i_node node, const serializer&);
			virtual sign_t sign_unit() const;

		public:
			sign_t sign() const;
			void save(o_node node, const serializer &s) const;
			void load(i_node node, const serializer &s);

			bool traversable() const;
			bool transparent() const;
			void act(environment &current);
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

			void accept(visitor&);

			void destroy();
			bool destroying() const;
		};
	}
}

#endif