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
		namespace
		{
			point inventory_start(const canvas &c) { return{ c.width() / 4, 1 }; }
			point inventory_range(const canvas &c) { return{ c.width() / 2, c.height() - 2 }; }
		}

		inventory_panel::inventory_panel(target_ptr target, canvas *ui_canvas) : panel(ui_canvas), m_target(target) {}
		inventory_panel::~inventory_panel() {}

		void inventory_panel::draw_panel()
		{
			point start = inventory_start(*m_canvas);
			point range = inventory_range(*m_canvas);

			m_canvas->rectangle(start, range, color(0.2, 0.2, 0.2, 0.5));
			m_canvas->rectangle(start, { range.X, 1 }, color(0.5, 0.5, 0.5, 0.75));
			m_canvas->write({ start.moved(range.X / 2 - strlen("inventory:") / 2, 0) }, "inventory:");

			int n = start.Y + 1;
			m_target->enumerate_items([&](target_t::item_t item)
			{
				m_canvas->write({ start.X + 1, n }, item->name());
				++n;
			});
		}
		bool inventory_panel::click_control(const point &position, unsigned int button)
		{
			point start = inventory_start(*m_canvas);
			point range = inventory_range(*m_canvas);
			if (position.in_range(start, range))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}