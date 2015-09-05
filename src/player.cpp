// name: player.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "player.h"

#include "game.h"
#include "delegate_action.h"

using namespace px;
using namespace px::rl;

player::player(receiver_t *receiver) : m_receiver(receiver)
{
	action_t::target_fn tf([&](user_t *user, target_t unit)
	{
		if (unit && receiver)
		{
			m_receiver->broadcast({ "psssch", 0xffffff, unit->position() });
		}
	});
	action_t::target_check_fn tfc([&](user_t *user, target_t unit)
	{
		return true;
	});
	m_skills.emplace_back(action_t(tf, tfc));
}
player::~player() {}

void player::acquire(item_t item)
{
	if (m_receiver)
	{
		m_receiver->broadcast(broadcast(std::string("+ ") + item->name(), 0xffffff, position()));
	}
}