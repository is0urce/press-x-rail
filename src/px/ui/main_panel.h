// name: stack_panel.h
// type: c++
// desc: class declaration
// auth: is0urce

// main container for panels

#ifndef PX_UI_MAIN_PANEL_H
#define PX_UI_MAIN_PANEL_H

#include <px/ui/stack_panel.h>

#include <memory>

namespace px
{
	namespace ui
	{
		class main_panel : public stack_panel
		{
		public:
			main_panel(canvas *ui_canvas);
			virtual ~main_panel();

		protected:
			virtual bool key_control(key_t code) override;
			
		private:
			void close_panels();

		public:
			void open_container();
		};
	}
}

#endif