// name: renderer.h
// type: c++ header file
// desc: opengl rendering
// auth: is0urce

// extends ogl with drawing functions

#pragma once

#include <px/shell/opengl.h>
#include <px/shell/vao.h>
#include <px/point.h>

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

			GLuint m_sampler;
			struct s_font
			{
				std::unique_ptr<font> font;
				GLuint texture;
			} m_ui, m_popup, m_glyph;

			struct
			{
				vao vao;
				GLuint program;
			} m_background, m_tiles, m_units, m_scene, m_light, m_notification, m_projectiles, m_uiback, m_uitext;

			GLuint m_light_frame, m_scene_frame;
			GLuint m_scene_texture, m_light_texture;
			unsigned int m_scene_size;

		private:
			void update_textures();
			void setup_scene();
			void fill_bg(const perception_t&);
			void fill_tiles(const perception_t&, font &tiles_font);
			void fill_units(const perception_t&, font &units_font);
			void fill_notifications(const perception_t &p, font &notify_font);
			void fill_projectiles(const perception_t &p, font &projectile_font, timespan_t span);
			void fill_ui(const canvas_t &gui, font &ui_font);

		public:
			renderer(opengl_handle opengl);
			virtual ~renderer();

			void draw(const perception_t &perception, const canvas_t &gui, double phase);
			void scale(double pan);
			void size(int &width, int &height);
			point world(const point &screen) const;
		};
	}
}