// name: astar.h
// type: c++ header
// desc: pathfinding
// auth: is0urce

#ifndef PX_ASTAR_H
#define PX_ASTAR_H

#include <px/point.h>

#include <memory>
#include <list>
#include <set>
#include <tuple>

namespace px
{
	class coord : public point
	{
	public:
		double g;
		double h;
		double f;
		coord* prev;

	public:
		coord(const point &point) : point(point), g(0), h(0), f(0), prev(0)
		{
		}
		coord(int x, int y) : point(x, y), g(0), h(0), f(0), prev(0)
		{
		}
		coord(point p, double heuristics) : point(p), g(0), h(heuristics), f(heuristics), prev(0)
		{
		}
		coord(int x, int y, double heuristics) : point(x, y), g(0), h(heuristics), f(heuristics), prev(0)
		{
		}
		void update(double cost, double heu, coord* x)
		{
			g = cost;
			h = heu;
			f = g + h;
			prev = x;
		}
	};

	class path
	{
	private:
		class score_less
		{
		public:
			bool operator() (std::shared_ptr<coord> a, std::shared_ptr<coord> b)
			{
				return (a->f) < (b->f); // reverse 'less' to get lowest
			}
		};
		class less
		{
		public:
			bool operator() (std::shared_ptr<coord> a, std::shared_ptr<coord> b)
			{
				return std::tie(a->X, a->Y) < std::tie(b->X, b->Y);
			}
		};
		static double heuristic(coord a, coord b)
		{
			return std::sqrt(std::pow(a.X - b.X, 2.0) + std::pow(a.Y - b.Y, 2.0));
		}
		static double distance(coord a, coord b)
		{
			return a.king_distance(b);
		}
		static std::unique_ptr<std::list<std::shared_ptr<coord>>> neighbours(coord c)
		{
			std::unique_ptr<std::list<std::shared_ptr<coord>>> result(new std::list<std::shared_ptr<coord>>);
			result->emplace_back(new coord(c.X + 1, c.Y));
			result->emplace_back(new coord(c.X, c.Y + 1));
			result->emplace_back(new coord(c.X - 1, c.Y));
			result->emplace_back(new coord(c.X, c.Y - 1));
			// diagonals
			result->emplace_back(new coord(c.X + 1, c.Y + 1));
			result->emplace_back(new coord(c.X + 1, c.Y - 1));
			result->emplace_back(new coord(c.X - 1, c.Y + 1));
			result->emplace_back(new coord(c.X - 1, c.Y - 1));

			return result;
		}
		static std::unique_ptr<std::list<coord>> construct_path(coord* step)
		{
			std::unique_ptr<std::list<coord>> path(new std::list<coord>);

			if (!step) throw std::runtime_error("px::path::construct_path() - step is null");

			while (true)
			{
				if (!step->prev) return path; // not count current position as part of path
				path->push_front(coord(step->X, step->Y));
				step = step->prev;
			}
		}

	public:
		static std::unique_ptr<std::list<coord>> find(int sx, int sy, int dx, int dy, unsigned int steps, std::function<bool(const point& p)> traversable)
		{
			return find(coord(sx, sy), coord(dx, dy), steps, traversable);
		}
		static std::unique_ptr<std::list<coord>> find(point start, point finish, unsigned int steps, std::function<bool(const point& p)> traversable)
		{
			return find(coord(start), coord(finish), steps, traversable);
		}
		static std::unique_ptr<std::list<coord>> find(coord start, coord finish, unsigned int steps, std::function<bool(const point& p)> traversable)
		{
			if (!traversable) throw std::logic_error("px::path::find() - traversable is null");

			std::set<std::shared_ptr<coord>, less> closed;
			std::multiset<std::shared_ptr<coord>, score_less> open;

			open.emplace(new coord(start, heuristic(start, finish)));

			while (steps > 0 && !open.empty())
			{
				auto xi = open.begin();
				auto x = *xi;

				// terminal
				if (*x == finish)
				{
					auto path = construct_path(x.get());
					return path;
				}

				open.erase(*xi);
				closed.insert(x);

				auto next = neighbours(*x);
				for (auto yi = next->begin(), end = next->end(); yi != end; ++yi)
				{
					auto y = *yi;
					if (!traversable(*y) && (*y != finish)) continue;
					if (closed.find(y) != closed.end()) continue;

					double cost = x->g + distance(*x, *y);
					y->update(cost, heuristic(*y, finish), x.get());

					bool find = false;
					for (auto it = open.begin(), end = open.end(); it != end; /* can't iterate if changed */)
					{
						if (**it == *y)
						{
							find = true;
							if (cost < (*it)->g)
							{
								open.erase(it); // update /w key value
								open.insert(y);
							}
							it = open.end();
						}
						else
						{
							++it;
						}
					}
					if (!find)
					{
						open.insert(y);
					}
				}

				--steps;
			}
			// nothing found

			// construct path to best found vertice in border set if present (i.e. maybe there is a path & we're just reach step limit)
			auto xi = open.begin();
			if (xi != open.end())
			{
				//if (H(**xi, finish) < H(start, finish))
				{
					return construct_path(xi->get());
				}
			}

			// we're in closed space, stay calm @ don't make mistake
			return nullptr;
		}
	};
}

#endif