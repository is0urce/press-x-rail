// name: serializer.h
// type: c++ header
// desc: shared unit serialization functions
// auth: is0urce

#ifndef PX_SERIALIZER_H
#define PX_SERIALIZER_H

#include "px/reader.h"
#include "px/writer.h"

#include <px/rl/player.h>
#include <px/rl/unit.h>
#include <px/rl/door.h>

#include <stdexcept>

namespace px
{
	namespace rl
	{
		class serializer
		{
		public:
			static void save(std::shared_ptr<unit> unit, writer::node_ptr node)
			{
				if (!node) throw std::logic_error("px::rl::serializer::save(player, node) - node is null");
				unit->save(node);
			}
			static std::shared_ptr<unit> load(reader::node &node)
			{
				std::shared_ptr<unit> u;
				auto name = node.name();
				if (name == "unit")
				{
					u.reset(new unit());
				}
				else if (name == "door")
				{
					u.reset(new door());
				}
				else
				{
					throw std::runtime_error("px::scene::load(..) unexpected element");
				}
				u->load(node);
				return u;
			}
		};
	}
}

#endif