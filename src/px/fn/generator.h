// name: generator.h
// type: c++ header
// desc: interface class declaration
// auth: is0urce

// px::fn::generator
// abstract generator class template
// uses operator() for filling referenced map with generated tiles provided by late-binded 'generate' overload
// _T - tile type, _U - unit type

#pragma once

#ifndef PX_FN_GENERATOR_H
#define PX_FN_GENERATOR_H

#include <px/map.h>
#include <px/point.h>

namespace px
{
	namespace fn
	{
		template <typename _T, typename _U>
		class generator
		{
		public:
			typedef map<_T> map_t;
			typedef std::function<void(_U, point)> fetch_op; // std::function<void(_U, point)>

		public:
			generator() {}
			virtual ~generator() {}

		protected:
			virtual void generate(map_t& map_reference, fetch_op fetch_fn) {}

		public:
			void operator()(map_t& map_reference, fetch_op fetch_fn)
			{
				generate(map_reference, fetch_fn);
			}
		};
	}
}

#endif