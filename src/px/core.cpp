// name: core.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "core.h"

#include <px/game.h>
#include <px/shell/renderer.h>
#include <px/shell/perception.h>
#include <px/timer.h>

#include <string>

namespace px
{
	namespace shell
	{
		namespace
		{
			const std::string autosave_path = "save\\autosave.sav";
			const std::string lastsave_path = "save\\current.sav";
		}

		core::core(opengl_handle ogl)
			:
			m_game(new game()),
			m_time(new timer()),
			m_graphics(new renderer(std::move(ogl))),
			m_version(0),
			m_shutdown(false)
		{
			std::srand((unsigned int)m_time->counter());
			auto turn = m_game->perception().version();
		}
		core::~core() {}

		void core::frame()
		{
			if (!finished())
			{
				// restart timer on new perception frame
				auto &perception = m_game->perception();
				auto current = perception.version();
				if (m_version != current)
				{
					m_version = current;
					m_time->restart();
				}

				auto &gui = m_game->canvas();

				// update interface
				int width, height;
				m_graphics->size(width, height);
				m_game->draw_ui(width / renderer::ui_cell_width, height / renderer::ui_cell_height);

				// drawcalls before windows destruction
				m_graphics->draw(perception, m_game->canvas(), m_time->measure());
			}
		}

		bool core::press(key vk)
		{
			bool result = m_game->panel()->key(vk);
			if (result)
			{
				return true;
			}
			else
			{
				switch (vk)
				{
				case key::command_cancel:
					shutdown();
					result = true;
					break;
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
					m_game->save(autosave_path);
					result = true;
					break;
				case key::quick_load:
					m_game->load(autosave_path);
					result = true;
					break;
				default:
					result = false;
					break;
				}
				return result;
			}
		}
		bool core::hover(const point &screen)
		{
			return m_game->panel()->hover(m_graphics->ui(screen)) || m_game->hover(m_graphics->world(screen));
		}
		bool core::click(const point &screen, unsigned int button)
		{
			return m_game->panel()->click(m_graphics->ui(screen), button) || m_game->click(m_graphics->world(screen), button);
		}
		bool core::scroll(int delta)
		{
			if (!m_game->panel()->scroll(delta))
			{
				m_graphics->scale(delta);
			}
			return true;
		}

		bool core::run() const
		{
			return !finished();
		}

		void core::shutdown(bool shutdown)
		{
			if (shutdown && !m_shutdown)
			{
				m_shutdown = true;
				m_game->save(lastsave_path);
			}
		}
		void core::shutdown()
		{
			shutdown(true);
		}
		bool core::finished() const
		{
			return m_shutdown;
		}
	}
}