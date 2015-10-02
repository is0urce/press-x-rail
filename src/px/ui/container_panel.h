// name: container_panel.h
// type: c++
// desc: class declaration
// auth: is0urce

#ifndef PX_UI_CONTAINER_PANEL_H
#define PX_UI_CONTAINER_PANEL_H

#include <px/ui/panel.h>
#include <px/rl/actor.h>
#include <px/rectangle.h>

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
			typedef rl::actor::user_t user_t;

		private:
			target_t *m_target; // inventory container
			user_t m_user;

			point m_hover;
			struct widget
			{
				rectangle outline;
				rectangle list;
				int scroll;
				int count;
			} m_inventory, m_container;

		public:
			container_panel(user_t user, target_t *container, canvas *ui_canvas);
			virtual ~container_panel();

		private:
			void update();
			void update(widget&, rectangle bounds);
			void scroll(widget&, int delta);

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