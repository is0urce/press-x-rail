// name: serializer.h
// type: c++ header
// desc: shared unit serialization functions
// auth: is0urce

#ifndef PX_SERIALIZER_H
#define PX_SERIALIZER_H

#include "px/reader.h"
#include "px/writer.h"

#include <stdexcept>
#include <functional>
#include <string>
#include <map>

namespace px
{
	namespace rl
	{
		class unit;
		class serializer
		{
		public:
			typedef reader::node in_node;
			typedef writer::node_ptr out_node;
			typedef std::shared_ptr<unit> element_ptr;
			typedef std::string signature;
			typedef std::function<element_ptr(const in_node&)> method_fn;

		private:
			std::map<std::string, method_fn> m_registered;

		public:
			serializer() {}
			virtual ~serializer() {}

		public:
			void register_method(signature sign, method_fn creation_method)
			{
				m_registered[sign] = creation_method;
			}
			template <typename _U>
			void register_method(signature sign)
			{
				register_method(sign, [this](const in_node& node)
				{
					element_ptr u = std::make_shared<_U>();
					u->load(node, *this);
					return u;
				});
			}
			template <typename _U>
			void register_method()
			{
				register_method<_U>(_U::signature());
			}

			void save(element_ptr unit, out_node node) const
			{
				if (!unit) throw std::logic_error("px::rl::serializer::save(unit, node) - unit is null");

				save(unit, node, unit->sign());
			}
			void save(element_ptr unit, out_node node, const signature &sign) const
			{
				if (!unit) throw std::logic_error("px::rl::serializer::save(unit, node) - unit is null");
				if (!node) throw std::logic_error("px::rl::serializer::save(unit, node) - node is null");

				unit->save(node->open(sign), *this);
			}
			element_ptr load(const in_node &node) const
			{
				signature name = io::to_string(node.name());
				auto find = m_registered.find(name);
				if (find == m_registered.end()) throw std::logic_error("px::rl::serializer::load(node) - node object not registered, signature=" + name);
				return load(node, find->second);
			}
			static element_ptr load(const in_node& node, method_fn create_method)
			{
				return create_method(node);
			}
		};
	}
}

#endif