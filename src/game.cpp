// name: game.cpp

#include "stdafx.h"

#include "game.h"

using namespace px;
using namespace px::shell;

namespace
{
	static const unsigned int perc_range = 10;
	static const point perc_center = point(perc_range, perc_range);
	static const point perc_reach = point(perc_range * 2 + 1, perc_range * 2 + 1);
}

const unsigned int game::perc_width = perc_range * 2 + 1;
const unsigned int game::perc_height = perc_range * 2 + 1;

game::game() : m_perception(perc_reach)
{
	m_player.reset(new rl::unit());
	m_player->appearance('@');

	m_scene.add(m_player, { 5, 5 });

	fill_perception();
}

game::~game() {}

bool game::step_control(const point &move)
{
	if (m_player)
	{
		m_player->position(m_player->position() + move);
		fill_perception();
	}
	return true;
}
bool game::command_control(key command)
{
	return true;
}

void game::fill_perception()
{
	if (!m_player) return;

	point start = m_player->position() - perc_center;
	m_perception.swap(start);

	// tiles
	m_perception.range().enumerate([&](const point &range_point)
	{
		auto &tile = m_scene.tile(start + range_point).appearance();
		m_perception.appearance(range_point, tile.image);
		m_perception.ground(range_point, tile.color);
		m_perception.light(range_point, color(1, 1, 1));
	});

	// units
	m_perception.add_unit(m_player->appearance(), m_player->position(), m_player->previous_position());
}

const perception& game::perception() const
{
	return m_perception;
}