// name: player.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "player.h"

#include <px/game.h>
#include <px/delegate_action.h>

#include <px/ui/main_panel.h>
#include <px/ui/container_panel.h>

namespace px
{
	namespace rl
	{

		player::player(receiver_t *receiver) : m_receiver(receiver) {}
		player::~player() {}

		void player::acquire(item_t item, bool silent)
		{
			if (!silent && m_receiver)
			{
				m_receiver->broadcast(broadcast(std::string("+ ") + item->name(), 0xffffff, position()));
			}
		}
		void player::access_container(container_t &container)
		{
			if (m_receiver)
			{
				auto menu = m_receiver->panel();
				if (menu)
				{
					menu->add("container", std::make_shared<ui::container_panel>(this, &container, menu->ui()));
					menu->open_container();
				}
			}
		}
	}
}