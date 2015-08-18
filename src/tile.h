// name: tile.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include <bitset>

namespace px
{
	namespace rl
	{
		template <typename _A, unsigned int layers = 1>
		class tile
		{
		public:
			typedef std::bitset<layers> bitset;

			// attributes
		private:
			_A m_appearance;
			bool m_transparent;
			bitset m_traversable;

			// ctor & dtor
		public:
			tile() : m_transparent(true) {}
			tile(bool is_transparent) : m_transparent(is_transparent) {}
			tile(bool is_transparent, bool is_traversable) : m_transparent(is_transparent) { traversable(is_traversable); }
			tile(bool is_transparent, bitset traversable) : m_transparent(is_transparent), m_traversable(traversable) {}
			~tile() {}

			// methods
		public:
			_A& appearance() { return m_appearance; }
			const _A& appearance() const { return m_appearance; }

			bool transparent() const { return m_transparent; }
			void transparent(bool val) { m_transparent = val; }

			bool traversable(unsigned int layer) const { return m_traversable.test(layer); }
			bool traversable() const { return traversable(0); }
			bool traversable(bitset l) const { return (m_traversable & l).any(); }
			void traversable(unsigned int layer, bool val) { m_traversable.set(layer, val); }
			void traversable(bool val) { traversable(0, val); }
			void traversable(bitset l, bool val)
			{
				if (val)
				{
					m_traversable |= l;
				}
				else
				{
					m_traversable &= ~l;
				}
			}

			void set_wall() { traversable(false); transparent(false); }
			void set_ground() { traversable(false); transparent(false); }
		};
	}
}