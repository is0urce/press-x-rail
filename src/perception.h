// name: perception.h
// type: c++ header
// desc: class declaration
// auth: is0urce

// "wiew" of game board

#pragma once

#include "point.h"
#include "map.h"
#include "appearance.h"
#include "avatar.h"
#include "notification.h"
#include "projectile.h"

#include <list>
#include <string>
#include <memory>
#include <functional>
#include <map>

namespace px
{
	namespace shell
	{
		class perception
		{
		public:
			typedef color ground_t;
			typedef appearance<unsigned int> tile_t;
			typedef appearance<unsigned int> appearance_t;
			typedef avatar<appearance_t> avatar_t;
			typedef std::list<avatar_t> unit_list;
			typedef std::function<void(const avatar_t&)> enum_fn;

		private:
			unsigned int m_version;
			map<tile_t> m_appearance;
			std::unique_ptr<map<ground_t>> m_ground;
			std::unique_ptr<map<ground_t>> m_ground_prev;
			std::unique_ptr<map<color>> m_color;
			std::unique_ptr<map<color>> m_color_prev;
			std::unique_ptr<map<bool>> m_hide;
			std::unique_ptr<map<bool>> m_hide_prev;
			point m_start;
			point m_start_prev;
			point m_move; // current-prev start
			unit_list m_units;
			std::multimap<point, notification, std::function<bool(const point&, const point&)>> m_notify;
			std::list<projectile> m_projectiles;

		public:
			perception(point range);
			perception(point range, point start);
			~perception();
		private:
			perception(const perception&); // disable copy
			void init(point range, point start);

		public:
			int width() const;
			int height() const;
			bool in_range(const point &position) const;
			const point& range() const;

			const tile_t& appearance(const point &position) const;
			const color& light(const point &position) const;
			const color& light_previous(const point &position) const;
			const ground_t& ground(const point &position) const;
			const ground_t& ground_previous(const point &position) const;
			bool hide(const point &position) const;

			void appearance(const point &position, const tile_t &tile);
			void light(const point &position, const color &color_value);
			void light_previous(const point &position, const color &color_value);
			void ground(const point &position, const ground_t &ground_value);
			void ground_previous(const point &position, const ground_t &ground_value);

			void add_unit(appearance_t appearance, point position, point position_previous);
			unit_list::size_type unit_count() const;
			void enumerate_units(enum_fn fn) const;

			void add_notification(notification::string_t text, color c, const point &position);
			void add_notification(notification::string_t text, color c, const point &position, double multiplier);
			void enumerate_notifications(std::function<void(const notification&)> fn) const;

			void add_projectile(projectile particle);
			void enumerate_projectiles(std::function<void(const projectile&)> fn) const;
			unsigned int projectile_count() const;

			void start(point point);
			point start() const;
			point start_previous() const;
			point movement() const;

			void swap(const point &start);
			unsigned int version() const;
		};
	}
}