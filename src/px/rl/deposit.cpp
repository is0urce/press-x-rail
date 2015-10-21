// name: deposit.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "deposit.h"

#include <px/game.h>

#include <px/rl/serializer.h>

#include <px/ui/main_panel.h>
#include <px/ui/container_panel.h>

namespace px
{
	namespace rl
	{
		deposit::deposit() : m_depleted(true), m_item(nullptr) {}
		deposit::deposit(resource_t resource) : m_depleted(false), m_item(resource) {}
		deposit::~deposit() {}

		unit::sign_t deposit::signature() { return "deposit"; }

		void deposit::renew(resource_t resource)
		{
			m_depleted = false;
			m_item = resource;
		}
		bool deposit::useable_unit(const environment&, user_t user) const
		{
			return !depleted();
		}
		void deposit::use_unit(environment&, user_t user) 
		{
			user->add_item(m_item);
			m_depleted = true;
			m_item.reset();
			m_appearance.color.shift_brightness(0.5);
			light().disable();
		}
		unit::sign_t deposit::sign_unit() const
		{
			return signature();
		}
		void deposit::serialize(o_node node, const serializer &s) const
		{
			unit::serialize(node, s);

			node->write("depleted", m_depleted);
			if (!m_depleted)
			{
				//s.save(m_item, node->open("res"));
			}
		}

		void deposit::deserialize(const i_node &node, const serializer &s)
		{
			unit::deserialize(node, s);

			node["depleted"] >> m_depleted;
			if (!m_depleted)
			{
				//m_item = std::dynamic_pointer_cast<rl::item, rl::unit>(s.load(node["res"]));
			}
		}

		bool deposit::depleted() const
		{
			return m_depleted;
		}
	}
}