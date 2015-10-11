// name: unit.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "unit.h"

namespace px
{
	namespace rl
	{

		unit::unit() : m_remove(false), m_invincible(false)
		{
		}

		unit::unit(const point &position) : m_position(position), m_prev_position(position), m_remove(false), m_invincible(false)
		{
		}

		unit::~unit()
		{
		}

		// virtual methods

		bool unit::traversable_unit() const
		{
			return false;
		}

		bool unit::transparent_unit() const
		{
			return true;
		}

		bool unit::useable_unit(const environment &current_environment, user_t user) const
		{
			return false;
		}

		void unit::use_unit(environment &current_environment, user_t user)
		{
		}

		void unit::apply_effect(effect &e)
		{
		}

		std::string unit::sign_unit() const
		{
			return signature();
		}

		std::string unit::sign() const
		{
			return sign_unit();
		}
		std::string unit::signature()
		{
			return "unit";
		}
		void unit::serialize(writer::node_ptr node, const serializer &s) const
		{
			node->write("x", m_position.X);
			node->write("y", m_position.Y);
			node->write("remove", m_remove);
			node->write("static", m_invincible);
			entity::store(node);
		}

		void unit::deserialize(const reader::node &node, const serializer &s)
		{
			node["x"] >> m_position.X;
			node["y"] >> m_position.Y;
			node["remove"] >> m_remove;
			node["static"] >> m_invincible;
			entity::restore(node);
		}

		// methods

		void unit::save(writer::node_ptr node, const serializer &s) const
		{
			serialize(node, s);
		}
		void unit::load(const reader::node &node, const serializer &s)
		{
			deserialize(node, s);
		}

		bool unit::traversable() const
		{
			return traversable_unit();
		}

		bool unit::transparent() const
		{
			return transparent_unit();
		}

		bool unit::useable(const environment &current_environment, user_t user) const
		{
			return useable_unit(current_environment, user);
		}

		void unit::use(environment &current_environment, user_t user)
		{
			use_unit(current_environment, user);
		}

		const point& unit::position() const
		{
			return m_position;
		}

		const point& unit::previous_position() const
		{
			return m_prev_position;
		}

		void unit::store_position()
		{
			m_prev_position = m_position;
		}

		void unit::position(const point& position)
		{
			m_position = position;
		}

		void unit::position(const point& position, const point& previous)
		{
			m_position = position;
			m_prev_position = previous;
		}

		void unit::destroy()
		{
			m_remove = true;
		}

		bool unit::destroying() const
		{
			return m_remove;
		}

		bool unit::invincible() const
		{
			return m_invincible;
		}

		void unit::invincible(bool is_invincible)
		{
			m_invincible = is_invincible;
		}

		void unit::apply(effect &e)
		{
			apply_effect(e);
		}
	}
}