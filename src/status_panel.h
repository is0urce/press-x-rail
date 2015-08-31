// name: status_panel.h
// type: c++
// desc: class declaration
// auth: is0urce

#pragma once

#include "panel.h"

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
			typedef std::shared_ptr<rl::person> target_ptr;

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