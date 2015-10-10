// name: renderer.h
// type: c++ header file
// desc: opengl rendering
// auth: is0urce

// extends ogl with drawing functions

#ifndef PX_SHELL_RENDERER_H
#define PX_SHELL_RENDERER_H

#include <px/shell/opengl.h>
#include <px/shell/vao.h>
#include <px/point.h>
#include <px/vector.h>

#include <memory>

namespace px
{
	namespace ui
	{
		class canvas;
	} 
	namespace shell
	{
		class font;
		class perception;

		class renderer
		{
		public:
			typedef std::unique_ptr<opengl> opengl_handle;
			typedef perception perception_t;
			typedef ui::canvas canvas_t;
			typedef double timespan_t;

		public:
			static const unsigned int ui_cell_width;
			static const unsigned int ui_cell_height;

		private:
			opengl_handle m_opengl;
			int m_width, m_height;
			double m_aspect;
			double m_scale;
			timespan_t m_last;
			timespan_t m_delta;
			vector m_center;
			double m_move_phase;

			GLuint m_sampler;
			struct s_font
			{
				std::unique_ptr<font> font;
				GLuint texture;
			} m_ui, m_popup, m_glyph;

			struct drawcall
			{
			public:
				vao vao;
				GLuint program;

			public:
				drawcall(const std::vector<unsigned int> &depths, GLuint program_id) : vao(depths), program(program_id) {}
			} m_background, m_tiles, m_units, m_notification, m_projectiles, m_light, m_lightmap, m_lightmap_prev, m_scene, m_lightdraw, m_uiback, m_uitext;

			GLuint m_scene_frame, m_light_frame, m_lightmap_frame; // framebuffers
			GLuint m_scene_texture, m_light_texture; // framebuffer textures for g-geometry and lights
			GLuint m_lightmap_texture, m_lightmap_prev_texture; // static lights textures
			unsigned int m_scene_size;
			unsigned int m_lightmap_size;

		private:
			void update_textures();
			void setup_scene();

			void fill_bg(const perception_t&);
			void fill_tiles(const perception_t&, font &tiles_font);
			void fill_units(const perception_t&, font &units_font);
			void fill_notifications(const perception_t &p, font &notify_font);
			void fill_projectiles(const perception_t &p, font &projectile_font, timespan_t span);
			void fill_ui(const canvas_t &gui, font &ui_font);
			void fill_lightmap(const perception_t &p);

			void draw_geometry(const perception_t &perception, double phase);
			void draw_light(const perception_t &perception, double phase);
			void draw_overlay(const perception_t &perception, const canvas_t &gui, double phase);

		public:
			renderer(opengl_handle opengl);
			virtual ~renderer();

			void draw(const perception_t &perception, const canvas_t &gui, double phase);
			void scale(double pan);
			void size(int &width, int &height);
			point world(const point &screen) const;
			point ui(const point &screen) const;
		};
	}
}

#endif