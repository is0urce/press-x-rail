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
	class library;
	namespace rl
	{
		class unit;
		class player;
		class serializer;
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
		typedef scene scene_t;
		typedef std::string file_path;

	public:
		static const unsigned int perc_width;
		static const unsigned int perc_height;

	private:
		shell::perception m_perception;
		std::shared_ptr<scene> m_scene;
		std::shared_ptr<world> m_world;
		std::shared_ptr<library> m_library;
		std::shared_ptr<rl::serializer> m_serializer;
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
		void generate_library();
		void fill_perception();
		target_ptr aquire_target(); // select target from hovering position
		bool useable(target_ptr target) const;
		void act(std::function<void()> action, bool turn);

	public:
		// settings
		unsigned int distance(const point &a, const point &b) const;

		// controls
		bool step(const point &move);
		bool cast(unsigned int cmd);
		bool cast(unsigned int cmd, target_ptr target);
		bool use();
		bool hover(point position);
		bool click(point position, unsigned int button);

		// accessors
		const shell::perception& perception() const;
		player_ptr player();
		const player_ptr player() const;
		target_ptr target();
		std::shared_ptr<scene> scene();

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