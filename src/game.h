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
		private:
			std::shared_ptr<rl::unit> m_player;
			perception m_perception;

		public:
			game() : m_perception(point(10, 10))
			{
				m_player.reset(new rl::unit());
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
				m_perception.range().enumerate([&](const point& position)
				{
					m_perception.ground(position, color(1.0, 0.0, 0.0));
				});
			}

		public:
			const perception& perception() const
			{
				return m_perception;
			}
		};
	}
}