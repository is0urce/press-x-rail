// name: status_panel.cpp
// type: c++ source file
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "status_panel.h"

#include "person.h"
#include "canvas.h"

using namespace px;
using namespace px::ui;

status_panel::status_panel(target_ptr target, canvas *ui_canvas) : panel(ui_canvas), m_target(target) {}
status_panel::~status_panel() {}

void status_panel::draw_panel()
{
	m_canvas->write({ 3, 3 }, "health:");
	m_canvas->write_integer({ 15, 3 }, m_target->health());
}