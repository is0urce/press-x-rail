// name: core.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_CORE_H
#define PX_CORE_H

#include <px/key.h>
#include <px/point.h>

#include <memory>

namespace px
{
	class game;
	class timer;
	namespace shell
	{
		class renderer;
		class opengl;
		class core
		{
		public:
			typedef std::unique_ptr<opengl> opengl_handle;

		private:
			std::unique_ptr<renderer> m_graphics;
			std::unique_ptr<game> m_game;
			std::unique_ptr<timer> m_time;
			bool m_shutdown;
			unsigned int m_version;

		public:
			core(opengl_handle ogl);
			virtual ~core();

			void frame();

			bool press(key vk);
			bool hover(const point &screen);
			bool click(const point &screen, unsigned int button);
			bool scroll(int delta);

			bool run() const;

			void shutdown(bool shutdown);
			void shutdown();
			bool finished() const;
		};
	}
}

#endif