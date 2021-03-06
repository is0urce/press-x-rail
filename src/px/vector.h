// name: vector.h
// type: c++ header
// desc: class with implementation
// auth: is0urce

// 2-d point with real components

#ifndef PX_VECTOR_H
#define PX_VECTOR_H

#include <px/coordinate.h>
#include <px/point.h>

namespace px
{
	class vector : public coordinate<double>
	{
	public:
		vector() : coordinate() {}
		vector(component x, component y) : coordinate(x, y) {}
		vector(const vector &vec2) : coordinate(vec2) {}
		vector(const point &point) : coordinate((component)point.X, (component)point.Y) {}

		vector operator-() const { return { -X, -Y }; }
		vector& operator+=(const vector &rhs) { move(rhs); return *this; }
		vector& operator-=(const vector &rhs) { move(-rhs); return *this; }
		vector operator*(component c) const { return { X * c, Y * c }; };
		vector operator*(const vector &v) const { return { X * v.X, Y * v.Y }; }
		vector operator/(component c) const { return { X / c, Y / c }; };
		vector operator/(const vector &v) const { return{ X / v.X, Y / v.Y }; };
		vector& operator*=(component c) { *this = *this * c; return *this; };
		vector& operator*=(const vector &v) { *this = *this * v; return *this; };
		vector& operator/=(component c) { *this = *this / c; return *this; };
		vector& operator/=(const vector &v) { *this = *this / v; return *this; };

		vector moved(const vector &move) const { vector result = *this; return result += move; }
		vector multiplied(const vector &c) const { return { X * c.X, Y * c.Y }; }
		vector multiplied(component w, component h) const { return multiplied({ w, h }); }
		vector multiplied(component c) const { return multiplied({ c, c }); }
		vector clamped(const vector &min, const vector &max) const { return vector((std::min)((std::max)(min.X, X), max.X), (std::min)((std::max)(min.Y, Y), max.Y)); }
		void normalize()
		{
			auto len = magnitude();
			if (len > 0) 
			{
				X /= len;
				Y /= len;
			}
			else
			{
				X = 0;
				Y = 0;
			}
		}
		vector normalized() { vector result = *this; result.normalize(); return result; }
		vector lerp(const vector &b, component t) const { return multiplied(1 - t).moved(b.multiplied(t)); }

		point floor() const { return { (point::component)std::floor(X), (point::component)std::floor(Y) }; }
		point ceil() const { return { (point::component)std::ceil(X), (point::component)std::ceil(Y) }; }
		point round() const { return { (point::component)std::round(X), (point::component)std::round(Y) }; }
	};

	inline vector operator+(vector lhs, const vector &rhs) { lhs += rhs; return lhs; }
	inline vector operator-(vector lhs, const vector &rhs) { lhs -= rhs; return lhs; }
}

#endif