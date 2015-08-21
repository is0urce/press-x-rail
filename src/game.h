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
			static const unsigned int perc_width;
			static const unsigned int perc_height;

		private:
			perception m_perception;
			rl::scene m_scene;
			std::shared_ptr<rl::unit> m_player;
			std::weak_ptr<rl::unit> m_target;
			point m_hover;

		public:
			game();
			~game();

		protected:
			virtual bool step_control(const point& move) override;
			virtual bool command_control(key command) override;
			virtual bool hover_control(point position) override;
			virtual bool click_control(point position, button_t button) override;

		private:
			void fill_perception();

		public:
			const perception& perception() const;
		};
	}
}