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
				if (p->visible() && p->key(code)) return true;
			}
			return false;
		}
		bool stack_panel::hover_control(const point &position)
		{
			for (auto p : m_stack)
			{
				if (p->visible() && p->hover(position)) return true;
			}
			return false;
		}
		bool stack_panel::click_control(const point &position, unsigned int button)
		{
			for (auto p : m_stack)
			{
				if (p->visible() && p->click(position, button)) return true;
			}
			return false;
		}
		bool stack_panel::scroll_control(int delta)
		{
			for (auto p : m_stack)
			{
				if (p->visible() && p->scroll(delta)) return true;
			}
			return false;
		}
		void stack_panel::draw_panel()
		{
			for (auto p : m_stack)
			{
				if (p->visible())
				{
					p->draw();
				}
			}
		}

		void stack_panel::add(panel_ptr panel)
		{
			m_stack.push_back(panel);
		}
	}
}