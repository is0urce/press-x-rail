// name: delegate_builder.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_FN_DELEGATE_BUILDER_H
#define PX_FN_DELEGATE_BUILDER_H

#include <px/fn/builder.h>

#include <functional>

namespace px
{
	namespace fn
	{
		template <typename _T, typename _U>
		class delegate_builder : public builder<_T, _U>
		{
		public:
			typedef std::function<void(map_t&, fetch_op)> delegate_t; // std::function<void(map_t&, fetch_op)>

		private:
			delegate_t m_function;

		public:
			delegate_builder(delegate_t fn) : m_function(fn) {}
			virtual ~delegate_builder() {}

		protected:
			virtual void generate(map_t &map_reference, fetch_op fetch_fn) override 
			{
				m_function(map_reference, fetch_fn);
			}
		};
	}
}

#endif