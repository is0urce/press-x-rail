#pragma once

#include "game_control.h"
#include "unit.h"

namespace px
{
	namespace shell
	{
		class game : public game_control
		{
		private:
			std::shared_ptr<rl::unit> m_player;

		public:
			game()
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
				}
				return true;
			}
			virtual bool command_control(key command) override
			{
				return true;
			}
		};
	}
}