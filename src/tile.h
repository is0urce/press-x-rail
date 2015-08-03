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
			// attributes
		private:
			_A m_appearance;
			bool m_transparent;
			std::bitset<layers> m_traversable;

			// ctor & dtor
		public:
			tile() : m_transparent(true) {}
			tile(bool is_transparent) : m_transparent(is_transparent) {}
			tile(bool is_transparent, bool is_traversable) : m_transparent(is_transparent) { traversable(is_traversable); }
			tile(bool is_transparent, std::bitset<layers> traversable) : m_transparent(is_transparent), m_traversable(traversable) {}
			~tile() {}

			// methods
		public:
			_A& appearance() { return m_appearance; }
			const _A& appearance() const { return m_appearance; }

			bool transparent() const { return m_transparent; }
			void transparent(bool val) { m_transparent = val; }

			bool traversable(unsigned int layer) { return m_traversable.test(layer); }
			bool traversable() { return traversable(0) }
			void traversable(unsigned int layer, bool val) { m_traversable.set(layer, val); }
			void traversable(bool val) { traversable(0, val); }
			void traversable(std::bitset<layers> val) { m_traversable = val; }

			void set_wall() { traversable(false); transparent(false); }
			void set_ground() { traversable(false); transparent(false); }
		};
	}
}