// name: game.cpp
// type: c++
// desc: class implementation
// auth: is0urce

// generate_library method implemented in game.lib.cpp

#include "stdafx.h"

#include "game.h"

#include <px/world.h>
#include <px/scene.h>
#include <px/library.h>

#include <px/rl/player.h>
#include <px/rl/serializer.h>
#include <px/rl/effect.h>

#include <px/ui/main_panel.h>
#include <px/ui/status_panel.h>
#include <px/ui/inventory_panel.h>

#include <px/writer.h>
#include <px/reader.h>

#include <px/shadowcasting.h>

#include <algorithm>

namespace px
{
	namespace
	{
		const unsigned int perc_range = 25;
		const point perc_center = point(perc_range, perc_range);
		const point perc_reach = point(perc_range * 2 + 1, perc_range * 2 + 1);
		const unsigned int action_distance = 1;
		const unsigned int light_range = 10;

		// light fade from distance
		double fade(double distance)
		{
			return 1 / (1 + distance * distance);
		}
	}

	const unsigned int game::perc_width = perc_range * 2 + 1;
	const unsigned int game::perc_height = perc_range * 2 + 1;

	// initial canvas size not really important, it should be resized next drawing pass
	game::game()
		:
		m_perception(perc_reach),
		m_canvas({ 1, 1 }),
		m_ui(std::make_shared<ui::main_panel>(&m_canvas)),
		m_fov_fn([&](const point &position) { return m_scene->transparent(position); })
	{
		// library
		generate_library();

		// register serializer objects
		m_serializer = std::make_shared<rl::serializer>(m_library);
		m_serializer->register_method<rl::deposit>();
		m_serializer->register_method<rl::unit>();
		m_serializer->register_method<rl::door>();
		m_serializer->register_method<rl::container>();
		m_serializer->register_method<rl::item>();
		m_serializer->register_method<rl::npc>();

		// scene, world and player
		m_world = std::make_shared<world>(m_library);
		m_scene = std::make_shared<scene_t>(m_world);
		m_player = std::make_shared<rl::player>(this);

		// setup player
		m_player->appearance({ '@', 0xffffff });
		m_player->fraction(1);
		m_player->health() = 100;
		m_player->light({ color(24.0f, 19.5f, 11.9f), true, true });

		m_player->add_skill(m_library->prototype<px::rl::person::action_t>("bite"));
		m_player->add_skill(m_library->prototype<px::rl::person::action_t>("teleport"));
		m_player->add_skill(m_library->prototype<px::rl::person::action_t>("heal"));

		// starting inventory
		for (int i = 0; i < 50; ++i)
		{
			auto itm = std::make_shared<rl::item>();
			itm->name(std::string("orb of idunno #") + std::to_string(i));
			m_player->add_item(itm, true);
		}
		m_player->weapon() = std::make_shared<rl::item>();
		m_player->weapon()->name() = "weapon of choice";

		// add player
		point player_pos(20, 20);
		m_scene->focus(player_pos);
		m_scene->add(m_player, player_pos);

		// ui
		m_ui->add("status", std::make_shared<ui::status_panel>(m_player, &m_canvas), true);
		m_ui->add("inventory", std::make_shared<ui::inventory_panel>(m_player, &m_canvas), false);

		fill_perception();
	}

	game::~game()
	{
	}

	unsigned int game::distance(const point &a, const point &b) const
	{
		return a.king_distance(b);
	}

	void game::act(std::function<void()> action, bool turn)
	{
		m_scene->enumerate_units([&](scene::unit_ptr unit) { unit->store_position(); });
		if (action)
		{
			action();
		}
		if (turn)
		{
			auto units = m_scene->units(); // copy to avoid iterator invalidation
			for (auto &u : units)
			{
				u.second->act(*this);
			}
			m_scene->tick(1);
		}
		fill_perception();
	}

	void game::fill_perception()
	{
		if (!m_player) return;

		point start = m_player->position() - perc_center;
		m_perception.swap(start);

		// units
		shadowcasting player_fov(m_fov_fn, m_player->position(), perc_range);
		map<color> lightmap(perc_reach, color(0, 0, 0));
		m_scene->enumerate_units([&](scene::unit_ptr unit)
		{
			// lightmap casting
			auto l = unit->light();
			point position = unit->position();
			if (l.enabled() && l.shadowcasting())
			{
				shadowcasting light_fov(m_fov_fn, unit->position(), light_range);
				perc_reach.enumerate([&](const point &range_point)
				{
					point absolute = start + range_point;
					// open spaces with both light and visible by player, exclude walls without objects (no doors or ore)
					if (light_fov.in_fov(absolute) && player_fov.in_fov(absolute) && (m_scene->transparent(absolute) || m_scene->blocking(absolute)))
					{
						lightmap.at(range_point) += l.color() * fade(position.distance(absolute));
					}
				});
			}

			// avatar
			if (player_fov.in_fov(position))
			{
				m_perception.add_avatar(unit->appearance(), position, unit->previous_position(), l, !unit->traversable());
			}
		});

		// tiles
		m_perception.range().enumerate([&](const point &range_point)
		{
			auto &tile = m_scene->tile(start + range_point).appearance();
			m_perception.appearance(range_point, tile.image);
			m_perception.ground(range_point, tile.color);
			m_perception.light(range_point, lightmap.at(range_point));
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
			auto blocking = m_scene->blocking(destination);
			if (blocking)
			{
				if (blocking->useable(*this, m_player))
				{
					act([&]()
					{
						blocking->use(*this, m_player);
					}, false);
				}
				else if (!blocking->invincible())
				{
					cast(0, blocking);
				}
			}
			else
			{
				if (m_scene->traversable(destination))
				{
					act([&]()
					{
						m_scene->focus(destination);
						m_scene->move(m_player, destination);
					}, true);
				}
			}
		}
		return true;
	}

	bool game::cast(unsigned int command)
	{
		if (m_player)
		{
			auto skill = m_player->skill(command);
			if (skill)
			{
				if (skill->targeted())
				{
					cast(command, aquire_target());
				}
				else
				{
					auto position = m_player->position() + m_hover;
					if (skill->useable(position))
					{
						act([&](){ skill->use(position); }, true);
					}
				}
			}
			return true;
		}
		return false;
	}

	bool game::cast(unsigned int command, target_ptr target)
	{
		if (m_player)
		{
			auto skill = m_player->skill(command);
			if (skill && skill->targeted() && target && !target->invincible() && skill->useable(target))
			{
				act([&]()
				{
					skill->use(target);
				}, true);
				return true;
			}
		}
		return false;
	}

	bool game::use()
	{
		auto target = aquire_target();
		if (useable(target))
		{
			act([&](){ target->use(*this, m_player); }, false);
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
				return use();
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
		return m_target = m_player ? m_scene->blocking(m_player->position() + m_hover) : nullptr;
	}

	bool game::useable(game::target_ptr target) const
	{
		return target && target->useable(*this, m_player) && distance(m_player->position(), target->position()) <= action_distance;
	}

	const shell::perception& game::perception() const
	{
		return m_perception;
	}

	game::player_ptr game::player()
	{
		return m_player;
	}
	const game::player_ptr game::player() const
	{
		return m_player;
	}
	std::shared_ptr<game::scene_t> game::scene()
	{
		return m_scene;
	}

	const ui::canvas& game::canvas() const
	{
		return m_canvas;
	}

	std::shared_ptr<ui::main_panel> game::panel() const
	{
		return m_ui;
	}

	void game::draw_ui(int width, int height)
	{
		if (width > 0 && height > 0)
		{
			m_canvas.resize(width, height);
			m_canvas.cls();
			m_ui->draw();
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
			m_scene->remove(m_player);

			writer save(path);

			save->write("version", 1);

			m_player->save(save->open("player"), *m_serializer);
			m_world->save(save->open("world"), *m_serializer);
			m_scene->save(save->open("scene"), *m_serializer);

			m_scene->add(m_player);

			broadcast(broadcast_t("autosaving...", 0xffffff, m_player->position()));
			act([&](){}, false);
		}
	}

	void game::load(file_path path)
	{
		reader file(path);

		m_scene->remove(m_player);

		m_player->load(file["player"], *m_serializer);
		m_world->load(file["world"], *m_serializer);
		m_scene->load(file["scene"], *m_serializer);

		m_scene->focus(m_player->position());
		m_scene->add(m_player);

		broadcast(broadcast_t("loaded autosave", 0xffffff, m_player->position()));
		act([&](){}, false);
	}
}