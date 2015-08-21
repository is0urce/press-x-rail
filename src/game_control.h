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
			virtual bool command_control(key command) = 0;
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
			bool command(key command_key)
			{
				return command_control(command_key);
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
					result = step(point(0, 1));
					break;
				case key::move_south:
					result = step(point(0, -1));
					break;
				case key::move_east:
					result = step(point(1, 0));
					break;
				case key::move_west:
					result = step(point(-1, 0));
					break;
				default:
					result = command(command_key);
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