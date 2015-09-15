// name: cave_builder.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_FN_CAVE_BUILDER_H
#define PX_FN_CAVE_BUILDER_H

#include <px/world.h>

#include <memory>

namespace px
{
	class library;
	namespace fn
	{
		class cave_builder : public builder<world::map_t, world::unit_ptr>
		{
		public:
			typedef std::shared_ptr<library> lib_ptr;

		private:
			lib_ptr m_library;

		public:
			cave_builder(lib_ptr lib);
			virtual ~cave_builder();

		protected:
			virtual void generate(map_t &map_reference, fetch_op fetch_fn) override;
		};
	}
}

#endif