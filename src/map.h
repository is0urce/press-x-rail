// name: map.h
// type: c++ header
// desc: class declaration
// auth: is0ruce

// generic 2-d container

#pragma once

#include "point.h"

#include <functional>
#include <memory>

namespace px
{
	template <typename _T>
	class map
	{
		// friend with maps with different element type
		template<typename _C>
		friend class map;

	public:
		typedef _T element;

		// attributes

	private:
		point m_range;
		std::unique_ptr<_T[]> m_tiles;

		// ctor & dtor

	public:
		map(point::component x, point::component y)
		{
			init(point(x, y));
		}
		map(point::component x, point::component y, const element &initial)
		{
			init(point(x, y));
			fill(initial);
		}
		map(point range)
		{
			init(range);
		};
		map(point range, const element &initial)
		{
			init(range);
			fill(initial);
		}
		map(point range, std::function<element(const point&)> generator_fn)
		{
			init(range);
			fill_indexed(generator_fn);
		};
		~map() {};
	private:
		map(const map&); // disable copy

	private:
		void init(point range) // constructor helper
		{
			if (range.X <= 0 || range.Y <= 0) throw std::runtime_error("map<_T>::init(point range) invalid range (x or y <= 0)");
			m_range = range;
			m_tiles.reset(new _T[range.size()]);
		}

		// methods

	public:
		auto size() -> decltype(m_range.size()) const { return m_range.size(); };
		point::component width() const { return m_range.X; };
		point::component height() const { return m_range.Y; };
		const point& range() const { return m_range; };
		bool in_range(const point &position) const { return position.in_range(m_range); };
		void swap(map &that)
		{
			std::swap(m_range, that.m_range);
			m_tiles.swap(that.m_tiles);
		}
		void resize(int w, int h)
		{
			resize(point(w, h));
		}
		void resize(const point &range)
		{
			if (m_range == range) return;

			swap(map(range));
		}
		const _T& at(const point &position, const _T& outer) const { return in_range(position) ? m_tiles[m_range.X * position.Y + position.X] : outer; }
		_T& at(const point &point, _T& outer) { return return in_range(position) ? m_tiles[m_range.X * position.Y + position.X] : outer; }
		const _T& at(const point& position) const
		{
			if (!in_range(position)) throw std::runtime_error("_T& map<_T>::at(point position) argument out of range");
			return m_tiles[m_range.X * position.Y + position.X];
		}
		_T& at(const point& position)
		{
			if (!in_range(position)) throw std::runtime_error("_T& map<_T>::at(point position) argument out of range");
			return m_tiles[m_range.X * position.Y + position.X];
		}
		void fill(const _T &tile)
		{
			int len = size();
			for (int i = 0; i < len; ++i)
			{
				m_tiles[i] = tile;
			}
		};
		void fill(const map &source)
		{
			if (source.m_range != m_range) throw std::runtime_error("void fill(const map &source) different ranges");

			int len = size();
			for (int i = 0; i < len; ++i)
			{
				m_tiles[i] = source.m_tiles[i];
			}
		}
		template<typename _C>
		void fill(const map<_C>& map, std::function<element(const _C&)> converter_fn)
		{
			if (map.m_range != m_range) throw std::runtime_error("void fill(const map<_C>& map, std::function<element(const _C&)> converter_fn) different ranges");
			if (!converter_fn) throw std::runtime_error("void fill(const map<_C>& map, std::function<element(const _C&)> converter_fn) - converter empty");

			int len = size();
			for (int i = 0; i < len; ++i)
			{
				m_tiles[i] = converter_fn(map.m_tiles[i]);
			}
		}
		void fill(std::function<element()> generator_fn)
		{
			if (!generator_fn) throw std::runtime_error("void fill(std::function<element()> generator_fn) - generator empty");

			int len = size();
			for (int i = 0; i < len; ++i)
			{
				m_tiles[i] = generator_fn();
			}
		};
		void fill_indexed(std::function<element(const point&)> generator_fn)
		{
			if (!generator_fn) throw std::runtime_error("void fill(std::function<element()> generator_fn) - generator empty");

			m_range.enumerate([&] (const point& position)
			{
				at(position) = generator_fn(position);
			});
		};
	};
}