// name: container_panel.h
// type: c++
// desc: class declaration
// auth: is0urce

#ifndef PX_UI_CONTAINER_PANEL_H
#define PX_UI_CONTAINER_PANEL_H

#include <px/ui/panel.h>

#include <memory>

namespace px
{
	namespace rl
	{
		class person;
		class container;
	}
	namespace ui
	{
		class container_panel : public panel
		{
		public:
			typedef rl::container target_t;
			typedef std::shared_ptr<target_t> target_ptr;
			typedef std::shared_ptr<rl::person> user_ptr;

		private:
			target_ptr m_target; // inventory container
			user_ptr m_user;
			point m_hover;
			point m_panel_start;
			point m_panel_range;
			point m_list_start;
			point m_list_range;
			int m_scroll; // first shown item offset in list
			int m_count; // number of items shown

		public:
			container_panel(user_ptr user, target_ptr container, canvas *ui_canvas);
			virtual ~container_panel();

		private:
			void update();
			void scroll_inventory(int delta);
			void scroll_container(int delta);

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