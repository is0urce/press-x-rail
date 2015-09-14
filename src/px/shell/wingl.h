// name: "wingl.h"
// type: c++ header
// desc: class declaration
// auth: is0urce

// windows-specific opengl graphics

#pragma once

#include "opengl.h"

#include "gl\glew.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <Windows.h>
#include <wingdi.h>

namespace px
{
	namespace shell
	{
		class wingl : public opengl
		{
		private:
			HWND m_hwnd;
			HDC m_hDC;
			HGLRC m_hRC; // ogl rendering context
			GLuint m_pixelformat;
			bool m_binded;

		public:
			wingl();
			wingl(HWND hwnd);
			virtual ~wingl();

		protected:
			virtual void update_screen(int &width, int &height) override;
			virtual void swap_buffers() override;

		public:
			void bind(HWND hwnd);
			void release();
			bool binded() const;
		};
	}
}