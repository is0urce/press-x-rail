// name: equipment.h
// type: c++ header
// desc: class
// auth: is0uce

#ifndef PX_RL_EQUIPMENT_H
#define PX_RL_EQUIPMENT_H

#include <memory>

namespace px
{
	namespace rl
	{
		class item;
		class equipment
		{
		public:
			typedef std::shared_ptr<item> item_ptr;

		private:
			item_ptr m_armor;
			item_ptr m_weapon;
			item_ptr m_helmet;
			item_ptr m_greaves;
			item_ptr m_boots;
			item_ptr m_gloves;

		public:
			equipment() {}
			virtual ~equipment() {}

		public:
			item_ptr& weapon() { return m_weapon; }
			item_ptr& armor() { return m_armor; }
			item_ptr& helmet() { return m_helmet; }
		};
	}
}

#endif