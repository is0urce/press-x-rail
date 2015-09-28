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
		container_panel::container_panel(user_t *user, target_t *container, canvas *ui_canvas)
			: panel(ui_canvas), m_user(user), m_target(container), m_scroll_container(0), m_scroll_inventory(0)
		{}
		container_panel::~container_panel() {}

		void container_panel::update()
		{
		}
		void container_panel::scroll_inventory(int delta)
		{
		}
		void container_panel::scroll_container(int delta)
		{
		}

		void container_panel::draw_panel()
		{
			m_canvas->write({ 5, 5 }, "CONTAINER OPENED");
		}
		bool container_panel::click_control(const point &position, unsigned int button)
		{
			return true;
		}
		bool container_panel::hover_control(const point &position)
		{
			return true;
		}
		bool container_panel::scroll_control(int delta)
		{
			return true;
		}
		bool container_panel::key_control(key_t code)
		{
			return true;
		}
	}
}