// name: point.h
// type: c++ header
// desc: class with implementation
// auth: is0urce

// 2-d point with integer components

#ifndef PX_POINT_H
#define PX_POINT_H

#include <px/coordinate.h>

#include <algorithm>
#include <functional>

namespace px
{
	class point : public coordinate<int>
	{
	public:
		point() : coordinate() {}
		point(component x, component y) : coordinate(x, y) {}

		point operator-() const { return { -X, -Y }; }
		point& operator+=(const point &rhs) { move(rhs); return *this; }
		point& operator-=(const point &rhs) { move(-rhs); return *this; }
		point operator*(component c) const { return { X * c, Y * c }; };
		point operator*(const point &c) const { return { X * c.X, Y * c.Y }; };
		point operator/(component c) const { return { X / c, Y / c }; };
		point operator/(const point &c) const { return { X / c.X, Y / c.Y }; };
		point& operator*=(component c) { *this = *this * c; return *this; };
		point& operator*=(const point &c) { *this = *this * c; return *this; };
		point& operator/=(component c) { *this = *this / c; return *this; };
		point& operator/=(const point &c) { *this = *this / c; return *this; };

		point moved(const point& move) const { point result = *this; return result += move; }
		point moved(component x, component y) const { return moved({ x, y }); }
		point multiplied(const point &c) const { return { X * c.X, Y * c.Y }; }
		point multiplied(component w, component h) const { return multiplied({ w, h }); }
		point multiplied(component c) const { return multiplied({ c, c }); }
		point clamped(const point &min, const point &max) const { return { (std::min)((std::max)(min.X, X), max.X), (std::min)((std::max)(min.Y, Y), max.Y) }; }
		point normalized() const
		{
			return{ (0 < X) - (X < 0), (0 < Y) - (Y < 0) };
		}

		void normalize() { *this = normalized(); }

		bool on_border(const point &p) const { return p.X == 0 || p.Y == 0 || p.X == X - 1 || p.Y == Y - 1; }

		// enumerators

		// enumerate rectangle from start to start + range
		void enumerate(const point &start, std::function<void(const point&)> enum_fn) const
		{
			point corner = start.moved(*this);
			for (int j = start.Y; j < corner.Y; ++j)
			{
				for (int i = start.X; i < corner.X; ++i)
				{
					enum_fn({ i, j });
				}
			}
		}
		// enumerable rectangle, this point value is range
		void enumerate(std::function<void(const point&)> enum_fn) const
		{
			enumerate(point(), enum_fn);
		}
	};

	inline point operator+(point lhs, const point &rhs) { lhs += rhs; return lhs; }
	inline point operator-(point lhs, const point &rhs) { lhs -= rhs; return lhs; }
}

#endif