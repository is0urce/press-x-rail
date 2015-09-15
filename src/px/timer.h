// name: Timer.h
// type: c++ header
// desc: class declaration & implementation

// time track

#ifndef PX_TIMER_H
#define PX_TIMER_H

#include <chrono>

namespace px
{
	class timer
	{
	public:
		typedef std::chrono::high_resolution_clock clock_t;
		typedef std::chrono::duration<float> fsec;

	private:
		clock_t::time_point m_last;

	public:
		timer() 
		{ 
			restart();
		}

		void restart()
		{
			m_last = clock_t::now();
		}

		// time in seconds
		double measure() const
		{
			return fsec(clock_t::now() - m_last).count();
		}

		auto counter() -> decltype(clock_t::now().time_since_epoch().count())
		{
			return clock_t::now().time_since_epoch().count();
		}
	};
}

#endif