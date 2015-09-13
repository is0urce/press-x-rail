// name: Timer.h
// type: c++ header
// desc: class declaration & implementation

// time track

#pragma once

#include <windows.h>

#include <stdexcept>

namespace px
{
	class timer
	{
	private:
		double m_freq;
		decltype(LARGE_INTEGER::QuadPart) m_start;

	public:
		timer() { restart(); }
		timer(bool start_timer) { if (start_timer) restart(); }
		~timer() {}

		void restart()
		{
			LARGE_INTEGER li;
			if (!QueryPerformanceFrequency(&li))
			{
				throw std::runtime_error("Timer::StartCounter() QueryPerformanceFrequency fails");
			}

			m_freq = double(li.QuadPart);

			QueryPerformanceCounter(&li);
			m_start = li.QuadPart;
		}

		// time in seconds
		double measure() const
		{
			LARGE_INTEGER li;
			QueryPerformanceCounter(&li);
			return (li.QuadPart - m_start) / m_freq;
		}

		// current processor ticks
		auto counter() -> decltype(LARGE_INTEGER::QuadPart) const
		{
			LARGE_INTEGER li;
			QueryPerformanceCounter(&li);
			return li.QuadPart;
		}
	};
}