// name: container_panel.h
// type: c++
// desc: class declaration
// auth: is0urce

#ifndef PX_UI_CONTAINER_PANEL_H
#define PX_UI_CONTAINER_PANEL_H

#include <px/ui/panel.h>
#include <px/rl/actor.h>

#include <memory>

namespace px
{
	namespace rl
	{
		class container;
	}
	namespace ui
	{
		class container_panel : public panel
		{
		public:
			typedef rl::actor::container_t target_t;
			typedef rl::actor user_t;

		private:
			target_t *m_target; // inventory container
			user_t *m_user;

			point m_hover;
			int m_scroll_inventory, m_scroll_container; // first shown item offset in list
			int m_count_inventory, m_count_container; // number of items shown

		public:
			container_panel(user_t *user, target_t *container, canvas *ui_canvas);
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