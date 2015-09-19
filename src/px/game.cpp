// name: game.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "game.h"

#include <px/rl/player.h>
#include <px/ui/status_panel.h>

#include "px/writer.h"
#include "px/reader.h"

using namespace px;

namespace
{
	static const unsigned int perc_range = 25;
	static const point perc_center = point(perc_range, perc_range);
	static const point perc_reach = point(perc_range * 2 + 1, perc_range * 2 + 1);
	static const unsigned int action_distance = 1;
}

const unsigned int game::perc_width = perc_range * 2 + 1;
const unsigned int game::perc_height = perc_range * 2 + 1;

// initial canvas size not really important, it should be resized next drawing pass
game::game()
	:
	m_perception(perc_reach),
	m_canvas({ 1, 1 })
{
	m_player.reset(new rl::player(this));
	m_player->appearance({ '@', 0xffffff });
	m_player->health() = 100;
	m_player->light({ color(24.0f, 19.5f, 11.9f), true });
	rl::person::action_t::target_fn tf([&](rl::person::caster_t *user, rl::person::target_t unit)
	{
		if (user)
		{
			//broadcast({ "puff!", 0xffffff, user->position() });
		}
		if (user && unit)
		{
			vector start = user->position();
			vector fin = unit->position();
			m_projectiles.push_back(projectile('*', 0xff0000, [=](projectile::timespan_t phase) { return start.lerp(fin, (std::min)(phase, 1.0)); }));
		}
	});
	rl::person::action_t::target_check_fn tfc([&](rl::person::caster_t *user, rl::person::target_t unit)
	{
		return true;
	});
	m_player->add_skill({ tf, tfc });
	rl::person::action_t::ground_fn ttf([&](rl::person::caster_t *user, const point &position)
	{
		m_scene.focus(position);
		m_scene.move(m_player, position);
	});
	rl::person::action_t::ground_check_fn ttfc([&](rl::person::caster_t *user, const point &position)
	{
		return true;// m_scene.traversable(position);
	});
	m_player->add_skill({ ttf, ttfc });

	m_scene.focus({ 18, 18 });
	m_scene.add(m_player, { 18, 18 });

	m_status.reset(new ui::status_panel(m_player, &m_canvas));

	fill_perception();
}

game::~game() {}

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
	m_scene.enumerate_units([&](scene::unit_ptr unit)
	{
		m_perception.add_avatar(unit->appearance(), unit->position(), unit->previous_position(), unit->light(), !unit->traversable());
	});

	// notifications
	for (auto &broadcast : m_broadcasts)
	{
		m_perception.add_notification(broadcast.text, broadcast.color, broadcast.position, broadcast.size);
	}
	m_broadcasts.clear();

	// projectiles
	for (auto &particle : m_projectiles)
	{
		m_perception.add_projectile(particle);
	}
	m_projectiles.clear();
}

bool game::step(const point &move)
{
	if (m_player)
	{
		point destination = m_player->position() + move;
		auto blocking = m_scene.blocking(destination);
		if (blocking)
		{
			if (blocking->useable(m_player))
			{
				blocking->use(m_player);
			}
			else if (!blocking->invincible())
			{
				// attack
			}
		}
		else
		{
			if (m_scene.traversable(destination))
			{
				m_scene.focus(destination);
				m_scene.move(m_player, destination);
			}
		}

		fill_perception();
	}
	return true;
}

bool game::action(unsigned int command)
{
	if (m_player)
	{
		auto skill = m_player->skill(command);
		if (skill)
		{
			if (skill->targeted())
			{
				auto target = aquire_target();
				if (target && !target->invincible() && skill->useable(target))
				{
					skill->use(target);
					fill_perception();
				}
			}
			else
			{
				auto position = m_player->position() + m_hover;
				if (skill->useable(position))
				{
					skill->use(position);
					fill_perception();
				}
			}
		}
		return true;
	}
	return false;
}

bool game::use()
{
	auto target = aquire_target();
	if (useable(target))
	{
		target->use(m_player);
		fill_perception();
	}
	return true;
}

bool game::click(point positon, unsigned int button)
{
	hover(positon);
	if (m_player && button == 1)
	{
		if (useable(m_target))
		{
			// use lmb
			return false;
		}
		else
		{
			int dx = (positon.X == 0 || std::abs(positon.X) < std::abs(positon.Y) / 2) ? 0 : positon.X > 0 ? 1 : -1;
			int dy = (positon.Y == 0 || std::abs(positon.Y) < std::abs(positon.X) / 2) ? 0 : positon.Y > 0 ? 1 : -1;
			return step({ dx, dy });
		}
	}
	return false;
}

bool game::hover(point position)
{
	m_hover = position;
	aquire_target();
	return true;
}

game::target_ptr game::aquire_target()
{
	return m_target = m_player ? m_scene.blocking(m_player->position() + m_hover) : nullptr;
}

bool game::useable(game::target_ptr target) const
{
	return target && target->useable(m_player) && m_player->position().king_distance(target->position()) <= action_distance;
}

const shell::perception& game::perception() const
{
	return m_perception;
}

game::player_ptr game::player()
{
	return m_player;
}

const ui::canvas& game::canvas() const
{
	return m_canvas;
}

void game::draw_ui(int width, int height)
{
	if (width > 0 && height > 0)
	{
		m_canvas.resize(width, height);
		m_canvas.cls();
		m_status->draw();
	}
}

void game::broadcast(broadcast_t message)
{
	m_broadcasts.emplace_back(message);
}

void game::save(file_path path)
{
	if (m_player)
	{
		m_scene.remove(m_player);

		writer save(path);
		m_player->save(save->open("player"));
		m_scene.save(save->open("scene"));

		m_scene.add(m_player);

		broadcast(broadcast_t("autosaving...", 0xffffff, m_player->position()));
		fill_perception();
	}
}

void game::load(file_path path)
{
	reader file(path);

	m_scene.remove(m_player);

	m_player->load(file["player"]["person"]);
	m_scene.load(file["scene"]);

	m_scene.focus(m_player->position());

	m_scene.add(m_player);

	broadcast(broadcast_t("loaded autosave", 0xffffff, m_player->position()));
	fill_perception();
}