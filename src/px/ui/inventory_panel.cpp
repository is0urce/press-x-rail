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
			const std::string title = "inventory";
			const std::string dots = ". . .";
			const std::string multiplier = " x";

			point inventory_start(const canvas &c) { return{ c.width() / 4, 1 }; }
			point inventory_range(const canvas &c) { return{ c.width() / 2, c.height() - 2 }; }
			point items_start(const point &start) { return start.moved(1, 2); }
			point items_range(const point &range) { return range.moved(-2, -3); }
			point element_range(const point &range) { return{ range.X, 1 }; }
			// _I - item
			template <typename _I>
			void enumerate_inventory(
				const point &list_start,
				const point &list_range,
				int scroll,
				const rl::inventory<_I> &target,
				std::function<void(const point &pen_start, const point &pen_range, _I item, unsigned int stack)> enum_fn)
			{
				point pen = list_start;
				point range = element_range(list_range);
				int current = 0;
				int count = list_range.Y / range.Y;

				target.enumerate_items([&](_I item)
				{
					int relative = current - scroll;
					if (relative >= 0 && relative < count)
					{
						enum_fn(pen, range, item, 1);
						pen.move(0, range.Y);
					}
					++current;
				});
			}
		}

		inventory_panel::inventory_panel(target_ptr target, canvas *ui_canvas)
			: panel(ui_canvas), m_target(target), m_scroll(0)
		{
			if (!target) throw std::logic_error("px::ui::inventory_panel::ctor() - target is null");
		}
		inventory_panel::~inventory_panel()
		{
		}

		void inventory_panel::update()
		{
			m_inventory.start = inventory_start(*m_canvas);
			m_inventory.range = inventory_range(*m_canvas);
			m_list.start = items_start(m_inventory.start);
			m_list.range = items_range(m_inventory.range);
			m_count = m_list.range.Y / element_range(m_list.range).Y;
			m_scroll = std::max(std::min(m_scroll, (int)m_target->item_count() - m_count), 0);
		}

		void inventory_panel::do_scroll(int delta)
		{
			m_scroll += delta > 0 ? 1 : -1;
		}

		void inventory_panel::draw_panel()
		{
			update();

			m_canvas->rectangle(m_inventory.start, m_inventory.range, color(0.2, 0.2, 0.2, 0.5));
			m_canvas->rectangle(m_inventory.start, { m_inventory.range.X, 1 }, color(0.5, 0.5, 0.5, 0.75));
			m_canvas->write({ m_inventory.start.moved(m_inventory.range.X / 2 - title.length() / 2, 0) }, title);

			enumerate_inventory<target_t::item_t>(m_list.start, m_list.range, m_scroll, *m_target,
				[&](const point &pen, const point &pen_range, target_t::item_t item, unsigned int stack)
			{
				if (m_hover.in_range(pen, pen_range))
				{
					m_canvas->rectangle(pen, pen_range, color(0.5, 0.5, 0.5, 0.75));
				}
				m_canvas->write(pen, item->name() + (stack > 1 ? multiplier + std::to_string(stack) : ""));
			});

			// trailing dots if list exceeds range
			if (m_scroll > 0)
			{
				m_canvas->write(m_inventory.start.moved(m_inventory.range.X / 2 - dots.length() / 2, 1), dots);
			}
			if (m_scroll + m_count < (int)m_target->item_count())
			{
				m_canvas->write(m_inventory.start.moved(m_inventory.range.X / 2 - dots.length() / 2, m_inventory.range.Y - 1), dots);
			}
		}
		bool inventory_panel::click_control(const point &position, unsigned int button)
		{
			update();
			m_hover = position;
			if (position.in_range(m_inventory.start, m_inventory.range))
			{
				target_t::item_t found;
				enumerate_inventory<target_t::item_t>(m_list.start, m_list.range, m_scroll, *m_target,
					[&](const point &pen, const point &pen_range, target_t::item_t item, unsigned int stack)
				{
					if (m_hover.in_range(pen, pen_range))
					{
						found = item;
					}
				});
				if (found)
				{
					found->activate(m_target);
				}
				return true;
			}
			else
			{
				disable();
				return true;
			}
		}
		bool inventory_panel::hover_control(const point &position)
		{
			update();
			m_hover = position;
			return m_hover.in_range(m_inventory.start, m_inventory.range);
		}
		bool inventory_panel::scroll_control(int delta)
		{
			update();
			if (m_hover.in_range(m_inventory.start, m_inventory.range))
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
			case key::move_south:
				do_scroll(1);
				break;
			case key::move_north:
				do_scroll(-1);
				break;
			default:
				result = false;
				break;
			}
			return result;
		}
	}
}