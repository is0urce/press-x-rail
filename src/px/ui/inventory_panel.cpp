// name: inventory_panel.cpp
// type: c++ source file
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "inventory_panel.h"

#include <px/rl/person.h>
#include <px/ui/canvas.h>
#include <px/key.h>

#include <algorithm>

namespace px
{
	namespace ui
	{
		namespace
		{
			point inventory_start(const canvas &c) { return{ c.width() / 4, 1 }; }
			point inventory_range(const canvas &c) { return{ c.width() / 2, c.height() - 2 }; }
			point items_start(const point &start) { return start.moved(1, 2); }
			point items_range(const point &range) { return range.moved(-2, -3); }
			point element_range(const point &range) { return{ range.X, 1 }; }
			const std::string title = "inventory";
			const std::string dots = ". . .";
		}

		inventory_panel::inventory_panel(target_ptr target, canvas *ui_canvas)
			: panel(ui_canvas), m_target(target), m_scroll(0)
		{
		}
		inventory_panel::~inventory_panel()
		{
		}

		void inventory_panel::update()
		{
			m_panel_start = inventory_start(*m_canvas);
			m_panel_range = inventory_range(*m_canvas);
			m_list_start = items_start(m_panel_start);
			m_list_range = items_range(m_panel_range);
			m_count = m_list_range.Y / element_range(m_list_range).Y;
			m_scroll = std::max(std::min(m_scroll, (int)m_target->item_count() - m_count), 0);
		}
		void inventory_panel::do_scroll(int delta)
		{
			m_scroll += delta > 0 ? 1 : -1;
		}

		void inventory_panel::draw_panel()
		{
			update();

			m_canvas->rectangle(m_panel_start, m_panel_range, color(0.2, 0.2, 0.2, 0.5));
			m_canvas->rectangle(m_panel_start, { m_panel_range.X, 1 }, color(0.5, 0.5, 0.5, 0.75));
			m_canvas->write({ m_panel_start.moved(m_panel_range.X / 2 - title.length() / 2, 0) }, title);

			point pen_start = m_list_start;
			point pen_range = element_range(m_list_range);
			int current = 0;
			if (m_scroll > 0)
			{
				m_canvas->write(pen_start.moved(pen_range.X / 2 - dots.length() / 2, -1), dots);
			}
			m_target->enumerate_items([&](target_t::item_t item)
			{
				int relative = current - m_scroll;
				if (relative >= 0 && relative < m_count)
				{
					if (m_hover.in_range(pen_start, pen_range))
					{
						m_canvas->rectangle(pen_start, pen_range, color(0.5, 0.5, 0.5, 0.75));
					}
					m_canvas->write(pen_start, item->name());
					pen_start.move(0, pen_range.Y);
				}
				++current;
			});
			if (m_scroll + m_count < (int)m_target->item_count())
			{
				m_canvas->write(m_panel_start.moved(m_panel_range.X / 2 - dots.length() / 2, m_panel_range.Y - 1), dots);
			}
		}
		bool inventory_panel::click_control(const point &position, unsigned int button)
		{
			update();
			m_hover = position;
			if (position.in_range(m_panel_start, m_panel_range))
			{
				target_t::item_t found;
				point pen_start = m_list_start;
				point pen_range = element_range(m_list_range);
				int current = 0;
				m_target->enumerate_items([&](target_t::item_t item)
				{
					int relative = current - m_scroll;
					if (relative >= 0 && relative < m_count)
					{
						if (m_hover.in_range(pen_start, pen_range))
						{
							found = item;
						}
						pen_start.move(0, pen_range.Y);
					}
					++current;
				});
				if (found)
				{
					found->activate(m_target);
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		bool inventory_panel::hover_control(const point &position)
		{
			update();
			m_hover = position;
			return m_hover.in_range(m_panel_start, m_panel_range);
		}
		bool inventory_panel::scroll_control(int delta)
		{
			update();
			if (m_hover.in_range(m_panel_start, m_panel_range))
			{
				do_scroll(-delta);
				return true;
			}
			return false;
		}
		bool inventory_panel::key_control(key_t code)
		{
			bool result = true;
			switch (code)
			{
			case key::command_cancel:
				m_scroll = 0;
				disable();
				break;
			//case key::move_south:
			//	do_scroll(1);
			//	break;
			//case key::move_north:
			//	do_scroll(-1);
			//	break;
			default:
				result = false;
				break;
			}
			return result;
		}
	}
}