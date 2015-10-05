// name: container_panel.h
// type: c++
// desc: class declaration
// auth: is0urce

#ifndef PX_UI_CONTAINER_PANEL_H
#define PX_UI_CONTAINER_PANEL_H

#include <px/ui/panel.h>
#include <px/rectangle.h>

#include <px/rl/actor.h>
#include <px/rl/container.h>

#include <memory>

namespace px
{
	namespace ui
	{
		class container_panel : public panel
		{
		public:
			typedef rl::container target_t;
			typedef rl::actor::user_t user_t;

		private:
			point m_hover;
			target_t *m_target; // inventory container
			user_t m_user;

			rectangle m_takeall; // 'take all' button bounds

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
			void update(widget&, const target_t::container_t &target, rectangle bounds);
			void scroll(widget&, int delta);
			void draw_widget(const widget&, const target_t::container_t &target, target_t::name_t name_text, color background_color, color title_color, color hover_color) const;
			void draw_list(const rectangle &item_bounds, target_t::item_t item, unsigned int stack, color hover) const;
			void take_all();

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