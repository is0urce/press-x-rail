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
		class actor;
	}
	class game : public shell::game_control
	{
	public:
		typedef std::shared_ptr<rl::actor> player_ptr;
		typedef std::shared_ptr<rl::unit> target_ptr;

	public:
		static const unsigned int perc_width;
		static const unsigned int perc_height;

	private:
		shell::perception m_perception;
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
		bool useable(target_ptr target) const;

	public:
		const shell::perception& perception() const;
		target_ptr target();
	};
}