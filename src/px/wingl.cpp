// name: wingl.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "wingl.h"

#include <stdexcept>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

namespace
{
	static PIXELFORMATDESCRIPTOR pfd =                  // pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),                  // Size Of This Pixel Format Descriptor
		1,                              // Version Number
		PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
		PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,                       // Must Support Double Buffering
		PFD_TYPE_RGBA,                          // Request An RGBA Format
		8,                               // Select Our Color Depth
		0, 0, 0, 0, 0, 0,                       // Color Bits Ignored
		0,                              // No Alpha Buffer
		0,                              // Shift Bit Ignored
		0,                              // No Accumulation Buffer
		0, 0, 0, 0,                         // Accumulation Bits Ignored
		16,                             // 16Bit Z-Buffer (Depth Buffer)
		0,                              // No Stencil Buffer
		0,                              // No Auxiliary Buffer
		PFD_MAIN_PLANE,                         // Main Drawing Layer
		0,                              // Reserved
		0, 0, 0                             // Layer Masks Ignored
	};
}

using namespace px;
using namespace px::shell;

wingl::wingl() : m_hwnd(0), m_hRC(0), m_binded(false)
{
}

wingl::wingl(HWND hwnd) : m_hwnd(0), m_hRC(0), m_binded(false)
{
	bind(hwnd);
}

wingl::~wingl()
{
	release();
}

void wingl::bind(HWND hwnd)
{
	release();

	m_hwnd = hwnd;
	if (!(m_hDC = GetDC(m_hwnd)))
	{
		throw std::runtime_error("GetDC failed");
	}
	if (!(m_pixelformat = ChoosePixelFormat(m_hDC, &pfd)))             // Did Windows Find A Matching Pixel Format?
	{
		throw std::runtime_error("ChoosePixelFormat failed");
	}
	if (!SetPixelFormat(m_hDC, m_pixelformat, &pfd))               // Are We Able To Set The Pixel Format?
	{
		throw std::runtime_error("SetPixelFormat failed");
	}
	if (!(m_hRC = wglCreateContext(m_hDC)))
	{
		throw std::runtime_error("wglCreateContext - can't create a GL Rendering Context");
	}
	if (!wglMakeCurrent(m_hDC, m_hRC))                        // Try To Activate The Rendering Context
	{
		throw std::runtime_error("wglMakeCurrent - can't activate the GL Rendering Context");
	}

	glewInit();
	m_binded = true;
}

void wingl::release()
{
	if (m_hRC)                                // Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))                 // Are We Able To Release The DC And RC Contexts?
		{
			throw std::runtime_error("Release Of DC And RC Failed.");
		}
		if (!wglDeleteContext(m_hRC))                 // Are We Able To Delete The RC?
		{
			throw std::runtime_error("Release Rendering Context Failed.");
		}
		m_hRC = 0;

		if (m_hDC)
		{
			ReleaseDC(m_hwnd, m_hDC);
			m_hDC = 0;
		}
	}
	m_binded = false;
}

void wingl::update_screen(int& w, int& h)
{
	RECT rect;
	GetClientRect(m_hwnd, &rect);
	w = rect.right - rect.left;
	h = rect.bottom - rect.top;
}

void wingl::swap_buffers()
{
	SwapBuffers(m_hDC);
}

bool wingl::binded() const
{
	return m_binded;
}