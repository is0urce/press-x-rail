// name: panel.h
// type: c++
// desc: base class declaration
// auth: is0urce

#ifndef PX_UI_PANEL_H
#define PX_UI_PANEL_H

#include <px/point.h>
#include <px/control.h>

#include <stdexcept>

namespace px
{
	namespace ui
	{
		class canvas;
		class panel : public control
		{
		protected:
			bool m_visible;
			canvas *m_canvas;

		public:
			panel(canvas *ui_canvas);
			virtual ~panel();

		protected:
			virtual void draw_panel() = 0;

		public:
			bool visible() const;
			void visible(bool enabled);
			void enable();
			void disable();
			bool toggle();
			void draw();
			canvas* ui();

			canvas* operator->();
		};
	}
}

#endif