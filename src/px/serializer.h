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
#include <functional>
#include <string>
#include <map>

namespace px
{
	namespace rl
	{
		class serializer
		{
		public:
			typedef reader::node in_node;
			typedef writer::node_ptr out_node;
			typedef std::shared_ptr<unit> element_ptr;
			typedef std::string signature;
			typedef std::function<element_ptr(const reader::node&)> method;

		private:
			std::map<std::string, std::function<element_ptr(const in_node&)>> m_registered;

		public:
			serializer();
			~serializer();

		public:
			void register_method(signature sign, method creation_method)
			{
				m_registered[sign] = creation_method;
			}
			template <typename _U>
			void register_method(signature sign)
			{
				register_method(sign, [](const in_node& node)
				{
					element_ptr u = std::make_shared<_U>();
					u->load(node);
					return u;
				});
			}

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