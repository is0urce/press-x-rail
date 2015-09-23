// name: inventory_panel.h
// type: c++
// desc: class declaration
// auth: is0urce

#ifndef PX_UI_INVENTORY_PANEL_H
#define PX_UI_INVENTORY_PANEL_H

#include <px/ui/panel.h>

#include <memory>

namespace px
{
	namespace rl
	{
		class actor;
	}
	namespace ui
	{
		class inventory_panel : public panel
		{
		public:
			typedef rl::actor target_t;
			typedef std::shared_ptr<target_t> target_ptr;

		private:
			target_ptr m_target;

		public:
			inventory_panel(target_ptr unit, canvas *ui_canvas);
			virtual ~inventory_panel();

		protected:
			virtual void draw_panel() override;
		};
	}
}

#endif