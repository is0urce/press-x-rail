// name: panel.cpp
// type: c++
// desc: abstract class methods implementation
// auth: is0urce

#include "stdafx.h"

#include "panel.h"
#include "canvas.h"

using namespace px;
using namespace px::ui;

panel::panel(canvas *ui_canvas) : m_canvas(ui_canvas), m_visible(true)
{
	if (!ui_canvas) throw std::logic_error("px::ui::panel::ctor(canvas) canvas is null");
}

panel::~panel() {}

void panel::draw()
{
	draw_panel();
}

bool panel::visible() const
{ 
	return m_visible;
}

void panel::visible(bool enabled)
{ 
	m_visible = enabled;
}

void panel::enable()
{ 
	visible(true);
}

void panel::disable()
{ 
	visible(false); 
}

bool panel::toggle()
{ 
	return m_visible = !m_visible;
}

canvas* panel::ui()
{
	return m_canvas;
}

canvas* panel::operator->()
{
	return m_canvas;
}