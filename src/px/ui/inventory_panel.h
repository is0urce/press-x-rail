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
		class person;
	}
	namespace ui
	{
		class inventory_panel : public panel
		{
		public:
			typedef rl::person target_t;
			typedef std::shared_ptr<target_t> target_ptr;

		private:
			target_ptr m_target;
			int m_scroll;
			int m_count;
			point m_hover;
			point m_panel_start;
			point m_panel_range;
			point m_list_start;
			point m_list_range;

		public:
			inventory_panel(target_ptr unit, canvas *ui_canvas);
			virtual ~inventory_panel();

		private:
			void update();
			void do_scroll(int delta);

		protected:
			virtual void draw_panel() override;
			virtual bool click_control(const point &position, unsigned int button) override;
			virtual bool hover_control(const point &position) override;
			virtual bool scroll_control(int delta) override;
			virtual bool key_control(key_t code) override;
		};
	}
}

#endif