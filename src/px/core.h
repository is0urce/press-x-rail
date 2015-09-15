// name: core.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include <px/game.h>
#include <px/key.h>
#include <px/shell/renderer.h>
#include <px/shell/game_control.h>
#include <px/shell/perception.h>
#include <px/timer.h>

namespace px
{
	namespace shell
	{
		class core
		{
		private:
			std::unique_ptr<renderer> m_graphics;
			game m_game;
			game_control m_control;

			unsigned int m_version = 0;
			px::timer m_time;

		public:
			core(renderer::opengl_handle ogl) : m_graphics(new renderer(std::move(ogl))), m_control(&m_game)
			{
				std::srand((unsigned int)m_time.counter());
				auto turn = m_game.perception().version();
			}
			virtual ~core() {}

			void frame()
			{
				if (!m_control.finished())
				{
					// restart timer on new perception frame
					auto &perception = m_game.perception();
					auto current = perception.version();
					if (m_version != current)
					{
						m_version = current;
						m_time.restart();
					}

					auto &gui = m_game.canvas();

					// update interface
					int width, height;
					m_graphics->size(width, height);
					m_game.draw_ui(width / renderer::ui_cell_width, height / renderer::ui_cell_height);

					// drawcalls before windows destruction
					m_graphics->draw(perception, m_game.canvas(), m_time.measure());
				}
			}

			void press(key vk)
			{
				if (vk == key::command_cancel) shutdown();

				m_control.press(vk);
			}
			void hover(int x, int y)
			{
				m_control.hover(m_graphics->world({ x, y }));
			}
			void click(int x, int y, unsigned int button)
			{
				m_control.click(m_graphics->world({ x, y }), button);
			}
			void scroll(int delta)
			{
				m_graphics->scale(delta);
			}

			bool run() const
			{
				return !m_control.finished();
			}

			void shutdown()
			{
				m_control.shutdown();
			}
		};
	}
}