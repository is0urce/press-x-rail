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
		point destination = m_player->position() + move;
		m_scene.focus(destination);
		m_scene.move(m_player, destination);

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

bool game::click_control(point positon, game::button_t button)
{
	hover_control(positon);
	if (m_player && button == 1)
	{
		int dx = (positon.X == 0 || std::abs(positon.X) < std::abs(positon.Y) / 2) ? 0 : positon.X > 0 ? 1 : -1;
		int dy = (positon.Y == 0 || std::abs(positon.Y) < std::abs(positon.X) / 2) ? 0 : positon.Y > 0 ? 1 : -1;
		return step_control({ dx, dy });
	}
	return false;
}

bool game::hover_control(point position)
{
	if (m_player)
	{
		m_hover = m_player->position() + position;
		m_target = m_scene.blocking(m_hover);
	}
	return true;
}