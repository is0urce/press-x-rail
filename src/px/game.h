// name: game.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_GAME_H
#define PX_GAME_H

#include <px/shell/perception.h>
#include "px/scene.h"
#include <px/ui/canvas.h>
#include <px/ui/panel.h>
#include "px/broadcast.h"
#include "px/projectile.h"

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
		shell::perception m_perception;
		scene m_scene;
		player_ptr m_player;
		target_ptr m_target;
		point m_hover;
		std::list<broadcast> m_broadcasts;
		std::list<projectile> m_projectiles;
		ui::canvas m_canvas;
		std::unique_ptr<ui::panel> m_ui;
		std::function<bool(const point &position)> m_fov_fn;

	public:
		game();
		virtual ~game();

	private:
		void fill_perception();
		target_ptr aquire_target(); // select target from hovering position
		bool useable(target_ptr target) const;

	public:
		// controls
		bool step(const point &move);
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
		ui::panel& panel() const;
		void draw_ui(int w, int h);

		// receiver
		void broadcast(broadcast_t message);

		// serialization
		void save(file_path path);
		void load(file_path path);
	};
}

#endif