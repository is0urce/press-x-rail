// name: game.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_GAME_H
#define PX_GAME_H

#include <px/shell/perception.h>
#include <px/ui/canvas.h>
#include "px/broadcast.h"
#include "px/projectile.h"

#include <memory>
#include <list>
#include <string>

namespace px
{
	class scene;
	class world;
	namespace rl
	{
		class unit;
		class player;
	}
	namespace ui
	{
		class main_panel;
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
		std::shared_ptr<scene> m_scene;
		std::shared_ptr<world> m_world;
		player_ptr m_player;
		target_ptr m_target;
		point m_hover;
		std::list<broadcast> m_broadcasts;
		std::list<projectile> m_projectiles;
		ui::canvas m_canvas;
		std::shared_ptr<ui::main_panel> m_ui;
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
		const player_ptr player() const;
		target_ptr target();

		// ui
		const ui::canvas& canvas() const;
		std::shared_ptr<ui::main_panel> panel() const;
		void draw_ui(int width, int height);

		// receiver
		void broadcast(broadcast_t message);

		// serialization
		void save(file_path path);
		void load(file_path path);
	};
}

#endif