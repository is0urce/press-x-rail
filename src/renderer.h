// name: renderer.h
// type: c++ header file
// desc: opengl rendering
// auth: is0urce

// extends ogl with drawing functions

#pragma once

#include <memory>

#include "opengl.h"
#include "vao.h"
#include "font.h"

namespace px
{
	namespace shell
	{

		class renderer
		{
		public:
			typedef std::unique_ptr<opengl> opengl_handle;

		private:
			opengl_handle m_opengl;
			int m_width, m_height;
			double m_aspect;
			double m_camera;
			font m_ui_font;
			font m_notify_font;
			font m_unit_font;

			vao m_background;
			GLuint m_program;

		private:
			void fill_bg();

		public:
			renderer(opengl_handle opengl);
			virtual ~renderer();

			void draw(double phase);
			void scale(float pan);
		};
	}
}