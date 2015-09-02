// name: player.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "player.h"

#include "game.h"

using namespace px;
using namespace px::rl;

player::player(receiver_t &receiver) : m_receiver(receiver) {}
player::~player() {}

void player::acquire(player::item_t item)
{
	m_receiver.broadcast(broadcast(std::string("+ ") + item->name(), 0xffffff, position()));
}