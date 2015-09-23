// name: inventory_panel.cpp
// type: c++ source file
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "inventory_panel.h"

#include <px/rl/actor.h>
#include <px/ui/canvas.h>

namespace px
{
	namespace ui
	{

		inventory_panel::inventory_panel(target_ptr target, canvas *ui_canvas) : panel(ui_canvas), m_target(target) {}
		inventory_panel::~inventory_panel() {}

		void inventory_panel::draw_panel()
		{
			m_canvas->write({ 0, 0 }, "inventory:");
			int n = 0;
			m_target->enumerate_items([&](target_t::item_t item)
			{
				m_canvas->write({ 12, n }, item->name());
				++n;
			});
		}
	}
}