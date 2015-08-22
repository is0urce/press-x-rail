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
		__int64 m_start;

	public:
		timer() { start(); }
		timer(bool start_timer) { if (start_timer) start(); }
		~timer() {}

		void start()
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

		// return time in seconds
		double counter() const
		{
			LARGE_INTEGER li;
			QueryPerformanceCounter(&li);
			return double(li.QuadPart - m_start) / m_freq;
		}
	};
}