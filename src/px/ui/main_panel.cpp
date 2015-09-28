// name: main_panel.cpp
// type: c++
// desc: class declaration
// auth: is0urce

// main container for panels

#include "stdafx.h"

#include "main_panel.h"


#include <px/key.h>

namespace px
{
	namespace ui
	{

		main_panel::main_panel(canvas *ui_canvas) : stack_panel(ui_canvas)
		{
		}
		main_panel::~main_panel() {}

		void main_panel::close_panels()
		{
			disable("inventory");
			disable("container");
		}
		bool main_panel::key_control(key_t code)
		{
			bool handled = true;
			if (!stack_panel::key_control(code))
			{
				switch (code)
				{
				case key::panel_inventory:
					{
						bool inventory = enabled("inventory");
						close_panels();
						if (!inventory)
						{
							enable("inventory");
						}
					}
					break;
				case key::move_none:
					close_panels();
					break;
				default:
					handled = false;
				}
			}
			return handled;
		}

		void main_panel::open_container()
		{
			enable("container");
		}
	}
}