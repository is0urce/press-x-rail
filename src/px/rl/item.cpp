// name: item.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "item.h"

namespace px
{
	namespace rl
	{

		item::item() : m_stackable(false), m_weight{}
		{
		}
		item::~item()
		{
		}

		unit::sign_t item::signature()
		{
			return "item";
		}

		void item::activate_item(activator_ptr activator)
		{
		}
		unit::sign_t item::sign_unit() const
		{
			return signature();
		}
		void item::serialize(o_node node, const serializer& s) const
		{
			node->write("stacking", m_stackable);
			node->write("weight", m_weight);
			unit::serialize(node, s);
		}
		void item::deserialize(const i_node &node, const serializer& s)
		{
			node["stacking"] >> m_stackable;
			node["weight"] >> m_weight;
			unit::deserialize(node, s);
		}
		void item::activate(activator_ptr activator)
		{ 
			activate_item(activator);
		}
		bool item::stackable() const
		{
			return m_stackable;
		}
		void item::stackable(bool is_stackable)
		{
			m_stackable = is_stackable;
		}
		item::weight_t item::weight() const
		{
			return m_weight;
		}
		void item::weight(weight_t weight_value)
		{
			m_weight = weight_value;
		}
	}
}