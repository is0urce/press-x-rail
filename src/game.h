#pragma once

#include "game_control.h"
#include "unit.h"
#include "perception.h"

namespace px
{
	namespace shell
	{
		class game : public game_control
		{
			static const unsigned int perc_range = 2;
			static const unsigned int perc_width = perc_range * 2 + 1;
			static const unsigned int perc_height = perc_range * 2 + 1;
		private:
			std::shared_ptr<rl::unit> m_player;
			perception m_perception;

		public:
			game() : m_perception(point(perc_width, perc_height), point(0, 0))
			{
				m_player.reset(new rl::unit());
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
					point position = start + range_point;
					auto& floor = position.X % 5 == 0  || position.Y % 5 == 0 ? color(1, 0, 0) : color(0, 0, 0);
					m_perception.ground(range_point, floor);
				});

				// units
				point position = m_player->position() - start;
				point position_prev = m_player->previous_position() - start;
				m_perception.add_unit(m_player->appearance(), position, position_prev);
			}

		public:
			const perception& perception() const
			{
				return m_perception;
			}
		};
	}
}