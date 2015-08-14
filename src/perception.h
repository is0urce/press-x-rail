// name: perception.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "point.h"
#include "map.h"
#include "color.h"

#include <list>
#include <string>
#include <memory>

namespace px
{
	namespace rl
	{
		class unit;
	}

	namespace shell
	{
		class perception
		{
		public:
			typedef color ground_t;
			typedef unsigned int appearance_t;
			typedef std::list<rl::unit> unit_list;
			//typedef UnitList::iterator unit_list_iterator;
			//typedef std::pair<UnitIterator, UnitIterator> UnitRange;

		private:
			map<appearance_t> m_appearance;
			std::unique_ptr<map<ground_t>> m_ground;
			std::unique_ptr<map<ground_t>> m_ground_prev;
			std::unique_ptr<map<color>> m_color;
			std::unique_ptr<map<color>> m_color_prev;
			point m_center;
			point m_center_prev;
			unit_list m_units;

		public:
			perception(const point& range);
			~perception();
		private:
			perception(const perception&); // disable copy

		public:
			int width() const;
			int height() const;
			bool in_range(const point &position) const;
			const point& range() const;

			const appearance_t& appearance(const point &position) const;
			const color& light(const point &position) const;
			const color& light_previous(const point &position) const;
			const ground_t& ground(const point &position) const;
			const ground_t& ground_previous(const point &position) const;

			void appearance(const point &position, const appearance_t &tile);
			void light(const point &position, const color &color_value);
			void light_previous(const point &position, const color &color_value);
			void ground(const point &position, const ground_t &ground_value);
			void ground_previous(const point &position, const ground_t &ground_value);

			//void add(Avatar avatar);
			//void AddUnit(Appearance appearance, Point position, Point previous);
			//const UnitList& GetUnits() const;

			//void AddNotification(Point position, Color color, NotifyString text);
			//const NotifyList& GetNotifications() const;

			//void SetStart(Point point);
			//Point GetStart() const;
			//Point GetMovement() const;
			//std::unique_ptr<Perception> Reassemble(const Point& movement);
		};
	}
}