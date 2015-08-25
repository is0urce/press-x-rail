// name: game_control.h

#pragma once

#include "point.h"
#include "key.h"

namespace px
{
	namespace shell
	{
		class  game_control
		{
		public:
			typedef unsigned int button_t;

		private:
			bool m_shutdown;

		public:
			game_control() : m_shutdown(false) {}
			virtual ~game_control() {}

		protected:
			virtual bool step_control(const point& move) = 0;
			virtual bool action_control(unsigned int cmd) = 0;
			virtual bool use_control() = 0;
			virtual bool hover_control(point position) = 0;
			virtual bool click_control(point position, button_t button) = 0;

		public:
			bool step(const point& move)
			{
				return step_control(move);
			}
			bool stay()
			{
				return step_control(point(0, 0));
			}
			bool action(unsigned int cmd)
			{
				return action_control(cmd);
			}
			bool use()
			{
				return use_control();
			}
			bool press(key command_key)
			{
				bool result;
				switch (command_key)
				{
				case key::move_none:
					result = stay();
					break;
				case key::move_north:
					result = step({ 0, 1 });
					break;
				case key::move_south:
					result = step({ 0, -1 });
					break;
				case key::move_east:
					result = step({ 1, 0 });
					break;
				case key::move_west:
					result = step({ -1, 0 });
					break;
				case key::move_southwest:
					result = step({ -1, -1 });
					break;
				case key::move_southeast:
					result = step({ 1, -1 });
					break;
				case key::move_northwest:
					result = step({ -1, 1 });
					break;
				case key::move_northeast:
					result = step({ 1, 1 });
					break;
				case key::action_use:
					result = use();
					break;
				case key::action0:
					result = action(0);
					break;
				case key::action1:
					result = action(1);
					break;
				case key::action2:
					result = action(2);
					break;
				case key::action3:
					result = action(3);
					break;
				case key::action4:
					result = action(4);
					break;
				case key::action5:
					result = action(5);
					break;
				default:
					result = false;
					break;
				}
				return result;
			}
			bool hover(point position)
			{
				return hover_control(position);
			}
			bool click(point position, button_t button)
			{
				return click_control(position, button);
			}

			void shutdown(bool shutdown)
			{
				m_shutdown = shutdown;
			}
			void shutdown()
			{
				shutdown(true);
			}
			bool finished() const
			{
				return m_shutdown;
			}
		};
	}
}