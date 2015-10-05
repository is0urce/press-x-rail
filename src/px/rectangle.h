// name: rectangle.h
// type: c++ header
// desc: class with implementation
// auth: is0urce

// 2-d rectangle with integer components

#ifndef PX_RECTANGLE_H
#define PX_RECTANGLE_H

#include <px/point.h>

namespace px
{
	class rectangle
	{
	public:
		point start;
		point range;

	public:
		rectangle() : start{}, range{} {}
		rectangle(point range_value) : start{}, range(range_value) {}
		rectangle(point start_point, point range_value) : start(start_point), range(range_value) {}

		bool border(const point &p) const { return p.X == start.X || p.Y == start.Y || p.X == start.X + range.X - 1 || p.Y == start.Y + range.Y - 1; }
		bool contains(const point &p) const { return p.X >= start.X && p.Y >= start.Y && p.X < start.X + range.X && p.Y < start.Y + range.Y; }

		// enumerate rectangle from start to start + range
		void enumerate(std::function<void(const point&)> enum_fn) const
		{
			point corner = start.moved(range);
			for (int j = start.Y; j < corner.Y; ++j)
			{
				for (int i = start.X; i < corner.X; ++i)
				{
					enum_fn({ i, j });
				}
			}
		}
	};
}

#endif