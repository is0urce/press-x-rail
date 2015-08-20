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

		private:
			opengl_handle m_opengl;
			int m_width, m_height;
			double m_camera;

			struct s_font
			{
				std::unique_ptr<font> font;
				GLuint texture;
			} m_ui, m_notify, m_glyph;

			struct
			{
				vao vao;
				GLuint program;
			} m_background, m_tiles, m_units, m_scene;

			GLuint m_lightmap, m_lightmap_min, m_scene_frame;
			GLuint m_scene_texture, m_lightmap_texture, m_lightmap_min_texture;
			unsigned int m_scene_size;

		private:
			void setup_scene();
			void fill_bg(const perception_t&);
			void fill_tiles(const perception_t&);
			void fill_units(const perception_t&);

		public:
			renderer(opengl_handle opengl);
			virtual ~renderer();

			void draw(const perception_t &perception, double phase);
			void scale(double pan);
		};
	}
}