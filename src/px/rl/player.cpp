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

		std::string player::signature()
		{
			return "player";
		}

		std::string player::sign_unit() const
		{
			return signature();
		}

		void player::acquire(item_t item, bool silent)
		{
			if (!silent && m_receiver)
			{
				m_receiver->broadcast(broadcast(std::string("+ ") + item->name(), 0xffffff, position()));
			}
		}
	}
}