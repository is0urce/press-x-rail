// name: builder.h
// type: c++ header
// desc: interface class declaration
// auth: is0urce

// px::fn::generator
// abstract generator class template
// uses build() or operator() for filling referenced map with late-binded 'generate' overload
// _T - map type, _U - unit type

#ifndef PX_FN_BUILDER_H
#define PX_FN_BUILDER_H

namespace px
{
	class point;

	namespace fn
	{
		template <typename _T, typename _U>
		class builder
		{
		public:
			typedef _T map_t;
			typedef std::function<void(_U, const point&)> fetch_op; // std::function<void(_U, point)>

		public:
			builder() {}
			virtual ~builder() {}

		protected:
			virtual void generate(map_t &map_reference, fetch_op fetch_fn) {}

		public:
			void build(map_t &map_reference, fetch_op fetch_fn)
			{
				generate(map_reference, fetch_fn);
			}
			void operator()(map_t &map_reference, fetch_op fetch_fn)
			{
				generate(map_reference, fetch_fn);
			}
		};
	}
}

#endif