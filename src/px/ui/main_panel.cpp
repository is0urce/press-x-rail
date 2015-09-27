// name: main_panel.cpp
// type: c++
// desc: class declaration
// auth: is0urce

// main container for panels

#include "stdafx.h"

#include "main_panel.h"

namespace px
{
	namespace ui
	{

		main_panel::main_panel(canvas *ui_canvas) : stack_panel(ui_canvas) {}
		main_panel::~main_panel() {}

		bool main_panel::key_control(key_t code)
		{
			return stack_panel::key_control(code);
		}

		void main_panel::open_container()
		{
			at("container")->enable();
		}
	}
}