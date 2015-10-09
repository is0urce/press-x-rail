// name: container_panel.cpp
// type: c++ source file
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "container_panel.h"

#include <px/key.h>
#include <px/ui/canvas.h>
#include <px/rl/actor.h>
#include <px/rl/container.h>

using namespace std::placeholders;

namespace px
{
	namespace ui
	{
		namespace
		{
			const std::string title = "You";
			const std::string dots = ". . .";
			const std::string multiplier = " x";
			const std::string takeall_text = "Take All (E)";

			rectangle inventory_bounds(const canvas &c) { return rectangle({ c.width() / 4, 1 }, { c.width() / 4, c.height() - 2 }); }
			rectangle container_bounds(const canvas &c) { return rectangle({ c.width() / 2, 1 }, { c.width() / 4, c.height() - 2 }); }
			rectangle item_bounds(const rectangle &outer) { return rectangle(outer.start.moved(1, 2), outer.range.moved(-2, -3)); }
			point element_range(const point &range) { return{ range.X, 1 }; }
			// _I - item
			template <typename _I>
			void enumerate_inventory(
				const rectangle &bounds,
				int scroll,
				const rl::inventory<_I> &target,
				std::function<void(const rectangle &item_bounds, _I item, unsigned int stack)> enum_fn)
			{
				point range = element_range(bounds.range);
				rectangle pen = rectangle(bounds.start, range);
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
			: panel(ui_canvas), m_user(user), m_target(container), m_hover{}
		{
			if (!user) throw std::logic_error("px::ui::container_panel::ctor() - user is null");
			if (!container) throw std::logic_error("px::ui::container_panel::ctor() - container is null");
			m_inventory.scroll = 0;
			m_container.scroll = 0;
		}
		container_panel::~container_panel() {}

		void container_panel::draw_list(const rectangle &bounds, target_t::item_t item, unsigned int stack, color hover) const
		{
			if (bounds.contains(m_hover))
			{
				m_canvas->rectangle(bounds, hover);
			}
			m_canvas->write(bounds.start, item->name() + (stack > 1 ? multiplier + std::to_string(stack) : ""));
		}

		void container_panel::take_all()
		{
			m_target->move_all(*m_user);
			disable();
		}
		void container_panel::update()
		{
			if (!m_user) throw std::logic_error("px::ui::container_panel::update() - user is null");
			if (!m_target) throw std::logic_error("px::ui::container_panel::update() - target is null");
			update(m_inventory, *m_user, inventory_bounds(*m_canvas));
			update(m_container, *m_target, container_bounds(*m_canvas));

			m_takeall = { { m_container.outline.start.X, m_container.outline.start.Y + m_container.outline.range.Y }, { m_container.outline.range.X, 1 } };
		}
		void container_panel::update(container_panel::widget &w, const target_t::container_t &target, rectangle bounds)
		{
			w.outline = bounds;
			w.list = item_bounds(w.outline);
			w.count = w.list.range.Y / element_range(w.list.range).Y;
			w.scroll = std::max(std::min(w.scroll, (int)target.item_count() - w.count), 0);
		}
		void container_panel::scroll(widget &w, int delta)
		{
			w.scroll += delta > 0 ? -1 : 1;
		}
		void container_panel::draw_widget(const widget& w, const target_t::container_t &target, target_t::name_t name_text, color back, color title_color, color hover_color) const
		{
			m_canvas->rectangle(w.outline, back);
			m_canvas->rectangle(w.outline.start, { w.outline.range.X, 1 }, title_color);
			m_canvas->write({ w.outline.start.moved(w.outline.range.X / 2 - name_text.length() / 2, 0) }, name_text);
			enumerate_inventory<target_t::item_t>(w.list, w.scroll, target, std::bind(&container_panel::draw_list, this, _1, _2, _3, hover_color));

			// dots
			if (w.scroll > 0)
			{
				m_canvas->write(w.outline.start.moved(w.outline.range.X / 2 - dots.length() / 2, 1), dots);
			}
			if (w.scroll + w.count < (int)target.item_count())
			{
				m_canvas->write(w.outline.start.moved(w.outline.range.X / 2 - dots.length() / 2, w.outline.range.Y - 1), dots);
			}
		}

		void container_panel::draw_panel()
		{
			update();

			draw_widget(m_inventory, *m_user, title, color(0.2, 0.2, 0.2, 0.5), color(0.5, 0.5, 0.5, 0.75), color(0.5, 0.5, 0.5, 0.75));
			draw_widget(m_container, *m_target, m_target->name(), color(0.2, 0.2, 0.2, 0.5), color(0.5, 0.5, 0.5, 0.75), color(0.5, 0.5, 0.5, 0.75));

			m_canvas->rectangle(m_takeall, color(1, 1, 1, 0.5));
			m_canvas->write(m_takeall.start, takeall_text, color(0.0, 0.0, 0.0, m_target->item_count() == 0 ? 0.5 : 1.0));
		}
		bool container_panel::click_control(const point &position, unsigned int button)
		{
			bool inside = false;

			// deposit
			if (m_inventory.outline.contains(m_hover))
			{
				inside = true;
				target_t::item_t found;
				enumerate_inventory<target_t::item_t>(m_inventory.list, m_inventory.scroll, *m_user, [&](const rectangle &item_bounds, target_t::item_t item, unsigned int stack)
				{
					if (item_bounds.contains(m_hover))
					{
						found = item;
					}
				});
				if (found)
				{
					m_user->move(found, *m_target);
				}
			}

			// pick up
			if (m_container.outline.contains(m_hover))
			{
				inside = true;
				target_t::item_t found;
				enumerate_inventory<target_t::item_t>(m_container.list, m_container.scroll, *m_target, [&](const rectangle &item_bounds, target_t::item_t item, unsigned int stack)
				{
					if (item_bounds.contains(m_hover))
					{
						found = item;
					}
				});
				if (found)
				{
					m_target->move(found, *m_user);
				}
			}

			// 'take all' button
			if (m_takeall.contains(m_hover))
			{
				inside = true;
				take_all();
			}

			if (!inside)
			{
				disable();
			}

			return true;
		}
		bool container_panel::hover_control(const point &position)
		{
			m_hover = position;
			return true;
		}
		bool container_panel::scroll_control(int delta)
		{
			if (m_inventory.outline.contains(m_hover))
			{
				scroll(m_inventory, delta);
			}
			if (m_container.outline.contains(m_hover))
			{
				scroll(m_container, delta);
			}
			return true;
		}
		bool container_panel::key_control(key_t code)
		{
			bool result = true;
			switch (code)
			{
			case key::command_cancel:
				m_inventory.scroll = 0;
				m_container.scroll = 0;
				disable();
				break;
			case key::action_use:
				take_all();
				break;
			default:
				result = false;
				break;
			}
			return result;
		}
	}
}