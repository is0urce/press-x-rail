// name: status_panel.h
// type: c++
// desc: class declaration
// auth: is0urce

#ifndef PX_UI_STATUS_PANEL_H
#define PX_UI_STATUS_PANEL_H

#include <px/ui/panel.h>

#include <memory>

namespace px
{
	namespace rl
	{
		class person;
	}
	namespace ui
	{
		class status_panel : public panel
		{
		public:
			typedef rl::person target;
			typedef std::shared_ptr<target> target_ptr;

		private:
			target_ptr m_target;

		public:
			status_panel(target_ptr unit, canvas *ui_canvas);
			virtual ~status_panel();

		protected:
			virtual void draw_panel() override;
		};
	}
}

#endif