// name: stack_panel.h
// type: c++
// desc: class declaration
// auth: is0urce

// main container for panels

#ifndef PX_UI_STACK_PANEL_H
#define PX_UI_STACK_PANEL_H

#include <px/ui/panel.h>

#include <memory>
#include <list>

namespace px
{
	namespace ui
	{
		class stack_panel : public panel
		{
		public:
			typedef std::shared_ptr<panel> panel_ptr;

		private:
			std::list<panel_ptr> m_stack;

		public:
			stack_panel(canvas *ui_canvas);
			virtual ~stack_panel();

		protected:
			virtual bool key_control(key_t code) override;
			virtual bool hover_control(const point &position) override;
			virtual bool click_control(const point &position, unsigned int button) override;
			virtual bool scroll_control(int delta) override;
			virtual void draw_panel() override;

		public:
			void add(panel_ptr panel);
		};
	}
}

#endif