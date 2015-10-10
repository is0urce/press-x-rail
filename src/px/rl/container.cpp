// name: container.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "container.h"

#include <px/game.h>

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
		void container::serialize(writer::node_ptr node) const
		{
			unit::serialize(node);
		}

		void container::deserialize(const reader::node &node)
		{
			unit::deserialize(node);
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