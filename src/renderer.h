// name: renderer.h
// type: c++ header file
// desc: opengl rendering
// auth: is0urce

// extends ogl with drawing functions

#pragma once

#include <memory>

#include "opengl.h"
#include "vao.h"

namespace px
{
	namespace shell
	{
		class font;

		class renderer
		{
		public:
			typedef std::unique_ptr<opengl> opengl_handle;

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
			} m_background, m_tiles, m_units;

		private:
			void fill_bg();
			void fill_units();

		public:
			renderer(opengl_handle opengl);
			virtual ~renderer();

			void draw(double phase);
			void scale(double pan);
		};
	}
}