// name: opengl.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "opengl.h"

using namespace px;
using namespace px::shell;

opengl::opengl()
{
}

opengl::~opengl()
{
}

void opengl::update(int &screen_width, int &screen_height)
{
	update_screen(screen_width, screen_height);
};

void opengl::swap()
{
	swap_buffers();
};