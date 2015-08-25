// name: game.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "game_control.h"

#include "perception.h"
#include "scene.h"

#include <memory>

namespace px
{
	namespace rl
	{
		class unit;
	}
	namespace shell
	{
		class game : public game_control
		{
		public:
			typedef std::shared_ptr<rl::unit> player_ptr;
			typedef std::weak_ptr<rl::unit> target_ptr;

		public:
			static const unsigned int perc_width;
			static const unsigned int perc_height;

		private:
			perception m_perception;
			rl::scene m_scene;
			player_ptr m_player;
			target_ptr m_target;
			point m_hover;

		public:
			game();
			~game();

		protected:
			virtual bool step_control(const point& move) override;
			virtual bool action_control(unsigned int cmd) override;
			virtual bool use_control() override;
			virtual bool hover_control(point position) override;
			virtual bool click_control(point position, button_t button) override;

		private:
			void fill_perception();
			target_ptr aquire_target(); // select target from hovering position

		public:
			const perception& perception() const;
			target_ptr target();
		};
	}
}