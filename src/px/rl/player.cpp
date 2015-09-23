// name: player.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "player.h"

#include <px/game.h>
#include <px/delegate_action.h>

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
	}
}