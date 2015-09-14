// name: "opengl.h"
// type: c++ header
// desc: class declaration
// auth: is0urce

// opengl graphics platform-independent abstraction
// platform-specific implementation must inherit from it

#pragma once

namespace px
{
	namespace shell
	{
		class opengl
		{
		public:
			opengl();
			virtual ~opengl();

		protected:
			virtual void update_screen(int &screen_width, int &screen_height) = 0;
			virtual void swap_buffers() = 0;

		public:
			void update(int &screen_width, int &screen_height);
			void swap();
		};
	}
}