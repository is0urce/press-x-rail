// name: coordinate.h
// type: c++ header
// desc: template class
// auth: is0urce

// template 2-d coordinate with generic methods

#pragma once

#include <algorithm>

namespace px
{
	template <typename _C>
	class coordinate
	{
	public:
		typedef _C component;

	public:
		component X;
		component Y;

	protected:
		coordinate() : X{}, Y{} {}
		coordinate(component x, component y) : X(x), Y(y) {}

	public:
		// accessors

		component x() const { return X; }
		component y() const { return Y; }

		// mutations

		void move(const coordinate &move) { X += move.X; Y += move.Y; }
		void move(component x, component y) { move({ x, y }); }
		void multiply(const coordinate &c) { X *= c.X; Y *= c.Y; }
		void multiply(component w, component h) { X *= w, Y *= h; }
		void multiply(component c) { multiply(c, c); }

		// distances

		component king_distance(const coordinate &another) const
		{
			return (std::max)(std::abs(X - another.X), std::abs(Y - another.Y));
		}
		component block_distance(const coordinate &another) const
		{
			return std::abs(X - another.X) + std::abs(Y - another.Y);
		}
		component distance2(const coordinate &p) const // squared distance
		{
			component dx = X - p.X;
			component dy = Y - p.Y;
			return dx * dx + dy * dy;
		}
		auto distance(const coordinate &another) -> decltype(std::sqrt(distance2(another))) const
		{
			return std::sqrt(distance2(another));
		}
		component magnitude2() const
		{
			return distance2(coordinate());
		}
		auto magnitude() -> decltype(std::sqrt(distance2(coordinate(0, 0)))) const
		{
			return distance2(coordinate());
		}

		// misc

		template <typename _M>
		void write(_M *memory) const { memory[0] = (_M)X; memory[1] = (_M)Y; }
		void write(component *memory) const { memory[0] = X; memory[1] = Y; }
		bool in_range(const coordinate &start, const coordinate &range) const { return !(X < start.X) && !(Y < start.Y) && X < start.X + range.X && Y < start.Y + range.Y; }
		bool in_range(const coordinate &range) const { return !(X < 0) && !(Y < 0) && X < range.X && Y < range.Y; }
		component size() const { return X * Y; }
	};

	// copmare operators
	template <typename _C>
	inline bool operator==(const coordinate<_C> &a, const coordinate<_C> &b) { return a.X == b.X && a.Y == b.Y; }
	template <typename _C>
	inline bool operator!=(const coordinate<_C> &a, const coordinate<_C> &b) { return !operator==(a, b); }
}