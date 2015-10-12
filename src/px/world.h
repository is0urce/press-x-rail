// name: game.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_WORLD_H
#define PX_WORLD_H

#include <px/reader.h>

#include <px/appearance.h>
#include <px/tile.h>
#include <px/map.h>
#include <px/fn/builder.h>

#include <memory>
#include <list>

namespace px
{
	namespace rl
	{
		class unit;
		class serializer;
	}

	class writer_node;
	class library;

	class world
	{
	public:
		typedef std::shared_ptr<writer_node> o_node;
		typedef reader::node i_node;
		typedef std::shared_ptr<library> library_ptr;
		typedef appearance<unsigned int> appearance_t;
		typedef tile<appearance_t> tile_t;
		typedef map<tile_t> map_t;
		typedef std::shared_ptr<rl::unit> unit_ptr;
		typedef std::list<unit_ptr> unit_list;
		typedef std::unique_ptr<map_t> map_ptr;
		typedef fn::builder<map_t, unit_ptr> builder_t;
		typedef std::unique_ptr<builder_t> builder_ptr;

	public:
		static const point cell_range;

	private:
		library_ptr m_library;

		map<bool> m_created;

		map<builder_ptr> m_landmarks;
		builder_ptr m_landmark_outer;

		map<unit_list> m_units;
		unit_list m_units_outher;

	public:
		world(library_ptr lib);
		virtual ~world();
		world(const world&) = delete;

	private:
		void generate_wall(map_t &cell_map, builder_t::fetch_op fetch);

	public:
		point cell(const point &absolute) const;

		library_ptr library();

		// management
		map_ptr generate(const point &cell, builder_t::fetch_op fetch);
		void store(unit_ptr unit);

		// serialization
		void save(o_node node, const rl::serializer& serializer_ref) const;
		void load(const i_node &nodec, const rl::serializer& serializer_ref);
	};
}

#endif