// name: status_panel.cpp
// type: c++ source file
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "status_panel.h"

#include <px/rl/person.h>
#include <px/ui/canvas.h>

#include <sstream>

namespace px
{
	namespace ui
	{

		status_panel::status_panel(target_ptr target, canvas *ui_canvas) : panel(ui_canvas), m_target(target) {}
		status_panel::~status_panel() {}

		void status_panel::draw_panel()
		{
			m_canvas->write({ 0, 0 }, "health:");
			m_canvas->write_integer({ 12, 0 }, 100);
			
			int line = 3;
			m_target->enumerate_affects([&](const target::status_t& s)
			{
				std::stringstream ss("");
				ss << s.name() << " (" << s.duration() << ")";
				m_canvas->write({ 0, line }, ss.str(), 0xffffff);
				++line;
			});
		}
	}
}