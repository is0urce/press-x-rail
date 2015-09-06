// name: scene.h
// type: c++ header
// desc: 'scene' class definition
// auth: is0urce

#pragma once

#include "reader.h"
#include "writer.h"

#include "player.h"

#include <stdexcept>

namespace px
{
	namespace rl
	{
		class serializer
		{
			// ctor & dtor
		public:
			serializer() {}
			virtual ~serializer() {}
		private:
			serializer(const serializer&) = delete;

			static void save(const unit &unit, writer::node_ptr node)
			{
				if (!node) throw std::logic_error("px::rl::serializer::save(player, node) - node is null");
				unit.save(node);
			}
			template <typename _T, typename _S>
			static std::shared_ptr<_T> load(_S &unit, reader::node &node)
			{
				unit.load(node);
				return unit;
			}
		};
	}
}