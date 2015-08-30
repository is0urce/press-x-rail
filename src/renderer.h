// name: renderer.h
// type: c++ header file
// desc: opengl rendering
// auth: is0urce

// extends ogl with drawing functions

#pragma once

#include <memory>

#include "opengl.h"
#include "vao.h"

#include "game.h"

namespace px
{
	namespace shell
	{
		class font;
		class perception;

		class renderer
		{
		public:
			typedef std::unique_ptr<opengl> opengl_handle;
			typedef perception perception_t;
			typedef double timespan_t;

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
			} m_background, m_tiles, m_units, m_scene, m_light, m_notification;

			GLuint m_light_frame, m_scene_frame;
			GLuint m_scene_texture, m_light_texture;
			unsigned int m_scene_size;

		private:
			void update_textures();
			void setup_scene();
			void fill_bg(const perception_t&);
			void fill_tiles(const perception_t&, font &tiles_font);
			void fill_units(const perception_t&, font &units_font);
			void fill_notifications(const perception_t&, font &notify_font);

		public:
			renderer(opengl_handle opengl);
			virtual ~renderer();

			void draw(const perception_t &perception, double phase);
			void scale(double pan);
			point world(const point &screen) const;
		};
	}
}