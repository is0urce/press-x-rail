// name: container.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "container.h"

#include <px/game.h>

#include <px/rl/serializer.h>

#include <px/ui/main_panel.h>
#include <px/ui/container_panel.h>

namespace px
{
	namespace rl
	{
		container::container()
		{
		}
		container::~container()
		{
		}

		unit::sign_t container::signature()
		{
			return "cont";
		}

		unit::sign_t container::sign_unit() const
		{
			return signature();
		}
		void container::serialize(o_node node, const serializer &s) const
		{
			unit::serialize(node, s);
			auto items = node->open("items");
			enumerate_items([&](item_t item)
			{
				s.save(item, items);
			});

		}

		void container::deserialize(const i_node &node, const serializer &s)
		{
			unit::deserialize(node, s);
			node["items"].enumerate([&](i_node item_node)
			{
				auto item = std::dynamic_pointer_cast<rl::item, rl::unit>(s.load(item_node));
				if (!item) throw std::runtime_error("px::container::deserialize(node, serializer) - container item cast failed");

				add_item(item, true);
			});
		}

		bool container::useable_unit(const environment& environment, user_t user) const
		{
			return user.get() == (actor*)environment.player().get();
		}
		void container::use_unit(environment &environment, user_t user)
		{
			if (user.get() == (actor*)environment.player().get())
			{
				auto menu = environment.panel();
				if (menu)
				{
					std::shared_ptr<ui::container_panel> cont_panel(new ui::container_panel(user, this, menu->ui()));
					menu->add("container", cont_panel);
					menu->open_container();
				}
			}
		}
	}
}