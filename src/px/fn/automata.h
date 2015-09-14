// name: Automation.h
// type: c++ header
// desc: template class for cellular automation
// auth: is0urce

// template for cellular automata based on map
// _C - cell / element type

#pragma once

#include "map.h"

namespace px
{
	template<typename _C>
	class automata : public map<_C>
	{
	public:

		automata(point::component x, point::component y) : map(x, y) {}
		automata(const point &range) : map(range) {}
		automata(point::component x, point::component y, const element &initial) : map(x, y, initial) {}
		automata(point range, const element &initial) : map(range, initial) {}
		~automata() {}

	private:

		// clip out-of-range access to bounds
		const _C& sample(const point &position) const
		{
			return at(position.clamped(point(0, 0), range().moved(-1, -1)));
		}

	public:
		template<typename _A>
		void execute(std::function<_A(_A, const _C& element)> fold, _A start, std::function<_C(_A)> select, unsigned int generations)
		{
			for (unsigned int i = 0; i < generations; ++i)
			{
				std::unique_ptr<map<_C>> generation(new map<_C>(range(), [&] (const point &pos)
				{
					_A acc = start;
					acc = fold(acc, sample(pos));
					acc = fold(acc, sample(pos.moved(0, 1)));
					acc = fold(acc, sample(pos.moved(0, -1)));
					acc = fold(acc, sample(pos.moved(-1, 0)));
					acc = fold(acc, sample(pos.moved(1, 0)));
					acc = fold(acc, sample(pos.moved(1, 1)));
					acc = fold(acc, sample(pos.moved(-1, -1)));
					acc = fold(acc, sample(pos.moved(1, -1)));
					acc = fold(acc, sample(pos.moved(-1, 1)));
					return select(acc);
				}));
				swap(*generation);
			}
		}

		// run once generator once
		template<typename _A>
		void execute(std::function<_A(_A, const _C& element)> fold, _A start, std::function<_C(_A)> select)
		{
			execute(fold, start, select, 1);
		}
	};
}

