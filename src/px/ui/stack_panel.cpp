// name: stack_panel.cpp
// type: c++
// desc: class declaration
// auth: is0urce

// main container for panels

#include "stdafx.h"

#include "stack_panel.h"

namespace px
{
	namespace ui
	{

		stack_panel::stack_panel(canvas *ui_canvas) : panel(ui_canvas) {}
		stack_panel::~stack_panel() {}

		bool stack_panel::key_control(key_t code)
		{
			for (auto p : m_stack)
			{
				if (p.second->visible() && p.second->key(code)) return true;
			}
			return false;
		}
		bool stack_panel::hover_control(const point &position)
		{
			for (auto p : m_stack)
			{
				if (p.second->visible() && p.second->hover(position)) return true;
			}
			return false;
		}
		bool stack_panel::click_control(const point &position, unsigned int button)
		{
			for (auto p : m_stack)
			{
				if (p.second->visible() && p.second->click(position, button)) return true;
			}
			return false;
		}
		bool stack_panel::scroll_control(int delta)
		{
			for (auto p : m_stack)
			{
				if (p.second->visible() && p.second->scroll(delta)) return true;
			}
			return false;
		}
		void stack_panel::draw_panel()
		{
			for (auto p : m_stack)
			{
				if (p.second->visible())
				{
					p.second->draw();
				}
			}
		}

		void stack_panel::add(panel_id name_tag, panel_ptr panel)
		{
			add(name_tag, panel, panel->visible());
		}

		void stack_panel::add(panel_id name_tag, panel_ptr panel, bool is_enabled)
		{
			remove(name_tag);
			panel->visible(is_enabled);
			m_stack.emplace(name_tag, panel);
		}

		void stack_panel::remove(const panel_id &name_tag)
		{
			auto find = m_stack.find(name_tag);
			if (find != m_stack.end())
			{
				m_stack.erase(find);
			}
		}

		stack_panel::panel_ptr stack_panel::at(const panel_id &name_tag)
		{
			auto find = m_stack.find(name_tag);
			if (find == m_stack.end()) throw std::logic_error("px::ui::stack_panel::at(panel_id) panel not found");
			return find->second;
		}
	}
}