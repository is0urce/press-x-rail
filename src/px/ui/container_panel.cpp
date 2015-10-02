// name: container_panel.cpp
// type: c++ source file
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "container_panel.h"

#include <px/ui/canvas.h>
#include <px/rl/actor.h>

namespace px
{
	namespace ui
	{
		namespace
		{
			const std::string title = "inventory";
			const std::string dots = ". . .";
			const std::string multiplier = " x";

			rectangle inventory_bounds(const canvas &c) { return rectangle({ c.width() / 4, 1 }, { c.width() / 4, c.height() - 2 }); }
			rectangle container_bounds(const canvas &c) { return rectangle({ c.width() / 2, 1 }, { c.width() / 4, c.height() - 2 }); }
			rectangle item_bounds(const rectangle &outer) {	return rectangle(outer.start.moved(1, 2), outer.range.moved(-2, -3)); }
			point element_range(const point &range) { return{ range.X, 1 }; }
			// _I - item
			template <typename _I>
			void enumerate_inventory(
				const rectangle &bounds,
				int scroll,
				const rl::inventory<_I> &target,
				std::function<void(const rectangle &item_bounds, _I item, unsigned int stack)> enum_fn)
			{
				rectangle pen = bounds;
				point range = element_range(bounds.range);
				int current = 0;
				int count = bounds.range.Y / range.Y;

				target.enumerate_items([&](_I item)
				{
					int relative = current - scroll;
					if (relative >= 0 && relative < count)
					{
						enum_fn(pen, item, 1);
						pen.start.move(0, range.Y);
					}
					++current;
				});
			}
		}

		container_panel::container_panel(user_t user, target_t *container, canvas *ui_canvas)
			: panel(ui_canvas), m_user(user), m_target(container)
		{
			m_inventory.scroll = 0;
			m_container.scroll = 0;
		}
		container_panel::~container_panel() {}

		void container_panel::update()
		{
			update(m_inventory, inventory_bounds(*m_canvas));
			update(m_container, container_bounds(*m_canvas));
		}
		void container_panel::update(container_panel::widget &w, rectangle bounds)
		{
			w.outline = bounds;
			w.list = item_bounds(w.outline);
			w.count = w.list.range.Y / element_range(w.list.range).Y;
			w.scroll = std::max(std::min(w.scroll, (int)m_target->item_count() - w.count), 0);
		}
		void container_panel::scroll(widget &w, int delta)
		{
			w.scroll += delta > 0 ? 1 : -1;
		}

		void container_panel::draw_panel()
		{
			update();
			m_canvas->write({ 5, 5 }, "CONTAINER OPENED");

			m_canvas->rectangle(m_inventory.outline, color(0.2, 0.2, 0.2, 0.5));
			m_canvas->rectangle(m_inventory.outline.start, { m_inventory.outline.range.X, 1 }, color(0.5, 0.5, 0.5, 0.75));
			m_canvas->write({ m_inventory.outline.start.moved(m_inventory.outline.range.X / 2 - title.length() / 2, 0) }, title);

			enumerate_inventory<target_t::item_t>(m_inventory.list, m_inventory.scroll, *m_target,
				[&](const rectangle &bounds, target_t::item_t item, unsigned int stack)
			{
				if (bounds.contains(m_hover))
				{
					m_canvas->rectangle(bounds, color(0.5, 0.5, 0.5, 0.75));
				}
				m_canvas->write(bounds.start, item->name() + (stack > 1 ? multiplier + std::to_string(stack) : ""));
			});
		}
		bool container_panel::click_control(const point &position, unsigned int button)
		{
			return false;
		}
		bool container_panel::hover_control(const point &position)
		{
			return false;
		}
		bool container_panel::scroll_control(int delta)
		{
			return false;
		}
		bool container_panel::key_control(key_t code)
		{
			return false;
		}
	}
}