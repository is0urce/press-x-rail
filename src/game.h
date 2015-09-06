// name: game.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "perception.h"
#include "scene.h"
#include "canvas.h"
#include "panel.h"
#include "broadcast.h"

#include <memory>
#include <list>
#include <string>

namespace px
{
	namespace rl
	{
		class player;
	}
	class game
	{
	public:
		typedef std::shared_ptr<rl::player> player_ptr;
		typedef std::shared_ptr<rl::unit> target_ptr;
		typedef broadcast broadcast_t;
		typedef std::string file_path;

	public:
		static const unsigned int perc_width;
		static const unsigned int perc_height;

	private:
		bool m_shutdown;
		shell::perception m_perception;
		rl::scene m_scene;
		player_ptr m_player;
		target_ptr m_target;
		point m_hover;
		std::list<broadcast> m_broadcasts;
		ui::canvas m_canvas;
		std::unique_ptr<ui::panel> m_status;

	public:
		game();
		virtual ~game();

	private:
		void fill_perception();
		target_ptr aquire_target(); // select target from hovering position
		bool useable(target_ptr target) const;

	public:
		// controls
		bool step(const point& move);
		bool action(unsigned int cmd);
		bool use();
		bool hover(point position);
		bool click(point position, unsigned int button);

		// accessors
		const shell::perception& perception() const;
		player_ptr player();
		target_ptr target();

		// ui
		const ui::canvas& canvas() const;
		void draw_ui(int w, int h);

		// receiver
		void broadcast(broadcast_t message);

		// serialization
		void save(file_path path);
		void load(file_path path);

		// shutdown
		void shutdown(bool shutdown);
		void shutdown();
		bool finished() const;
	};
}