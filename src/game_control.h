// name: game_control.h
// type: c++ header
// desc: class
// auth: is0uce

#pragma once

#include  "game.h"

#include "point.h"
#include "key.h"

#include <string>

namespace px
{
	namespace shell
	{
		class game_control
		{
		public:
			typedef unsigned int button_t;

		private:
			game *m_game;

		public:
			game_control(game *game_ptr) : m_game(game_ptr) 
			{
				if (!game_ptr) throw std::logic_error("game_control::ctor() game is null");
			}
			virtual ~game_control() {}

		public:
			bool press(key command_key)
			{
				bool result;
				switch (command_key)
				{
				case key::move_none:
					result = m_game->step({ 0, 0 });
					break;
				case key::move_north:
					result = m_game->step({ 0, 1 });
					break;
				case key::move_south:
					result = m_game->step({ 0, -1 });
					break;
				case key::move_east:
					result = m_game->step({ 1, 0 });
					break;
				case key::move_west:
					result = m_game->step({ -1, 0 });
					break;
				case key::move_southwest:
					result = m_game->step({ -1, -1 });
					break;
				case key::move_southeast:
					result = m_game->step({ 1, -1 });
					break;
				case key::move_northwest:
					result = m_game->step({ -1, 1 });
					break;
				case key::move_northeast:
					result = m_game->step({ 1, 1 });
					break;
				case key::action_use:
					result = m_game->use();
					break;
				case key::action0:
					result = m_game->action(0);
					break;
				case key::action1:
					result = m_game->action(1);
					break;
				case key::action2:
					result = m_game->action(2);
					break;
				case key::action3:
					result = m_game->action(3);
					break;
				case key::action4:
					result = m_game->action(4);
					break;
				case key::action5:
					result = m_game->action(5);
					break;
				case key::quick_save:
					m_game->save("save\\autosave.sav");
					result = true;
					break;
				case key::quick_load:
					m_game->load("save\\autosave.sav");
					result = true;
					break;
				default:
					result = false;
					break;
				}
				return result;
			}
			bool hover(point position)
			{
				return m_game->hover(position);
			}
			bool click(point position, button_t button)
			{
				return m_game->click(position, button);
			}
		};
	}
}