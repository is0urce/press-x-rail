#pragma once

#include "game_control.h"

#include "perception.h"
#include "scene.h"
#include "unit.h"

#include <memory>

namespace px
{
	namespace shell
	{
		class game : public game_control
		{
		public:
			static const unsigned int perc_range = 10;
			static const unsigned int perc_width = perc_range * 2 + 1;
			static const unsigned int perc_height = perc_range * 2 + 1;

		private:
			rl::scene m_scene;
			std::shared_ptr<rl::unit> m_player;
			perception m_perception;

		public:
			game() : m_perception(point(perc_width, perc_height), point(0, 0))
			{
				m_player.reset(new rl::unit());
				m_player->appearance('@');

				m_scene.add(m_player, point(5, 5));

				fill_perception();
			}
			~game() {}

		protected:
			virtual bool step_control(const point& move) override
			{
				if (m_player)
				{
					m_player->position(m_player->position() + move);
					fill_perception();
				}
				return true;
			}
			virtual bool command_control(key command) override
			{
				return true;
			}

		private:
			void fill_perception()
			{
				if (!m_player) return;

				point start = m_player->position() - point(perc_range, perc_range);
				m_perception.swap(start);

				// tiles
				m_perception.range().enumerate([&](const point& range_point)
				{
					auto &tile = m_scene.tile(start + range_point).appearance();
					m_perception.appearance(range_point, tile.image);
					m_perception.ground(range_point, tile.color);
					m_perception.light(range_point, color(1, 1, 1));
				});

				// units
				m_perception.add_unit(m_player->appearance(), m_player->position(), m_player->previous_position());
			}

		public:
			const perception& perception() const
			{
				return m_perception;
			}
		};
	}
}