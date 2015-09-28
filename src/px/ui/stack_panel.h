// name: stack_panel.h
// type: c++
// desc: class declaration
// auth: is0urce

// generic container for panels

#ifndef PX_UI_STACK_PANEL_H
#define PX_UI_STACK_PANEL_H

#include <px/ui/panel.h>

#include <memory>
#include <map>
#include <string>

namespace px
{
	namespace ui
	{
		class stack_panel : public panel
		{
		public:
			typedef std::shared_ptr<panel> panel_ptr;
			typedef std::string panel_id;
			typedef std::map<panel_id, panel_ptr> stack_t;

		private:
			stack_t m_stack;

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
			void add(panel_id name_tag, panel_ptr panel);
			void add(panel_id name_tag, panel_ptr panel, bool start_enabled);
			void remove(const panel_id &name_tag);
			void disable(const panel_id &name_tag);
			void enable(const panel_id &name_tag);
			void toggle(const panel_id &name_tag);
			bool enabled(const panel_id &name_tag) const;
			panel_ptr at(const panel_id &name_tag);
		};
	}
}

#endif