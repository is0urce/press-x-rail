// name: player.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "player.h"

#include "game.h"
#include "delegate_ability.h"

using namespace px;
using namespace px::rl;

namespace
{
	typedef delegate_ability<character*, std::shared_ptr<unit>> skill_t;
}

player::player(receiver_t &receiver) : m_receiver(receiver)
{
	m_skills.reserve(12);
	skill_t::target_fn tf([&](character* user, std::shared_ptr<unit> unit)
	{
		if (unit)
		{
			m_receiver.broadcast({ "psssch", 0xffffff, unit->position() });
		}
	});
	skill_t::target_check_fn tfc([&](character* user, std::shared_ptr<unit> unit)
	{
		return true;
	});
	m_skills.emplace_back((ability_t*)new skill_t(this, tf, tfc));
}
player::~player() {}

void player::acquire(player::item_t item)
{
	m_receiver.broadcast(broadcast(std::string("+ ") + item->name(), 0xffffff, position()));
}