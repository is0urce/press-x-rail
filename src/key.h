// name: key.h
// type: c++ header
// desc: system virtual key enum
// auth: is0urce

namespace px
{
	namespace shell
	{
		enum class key : int
		{
			direction_north,
			direction_south,
			direction_west,
			direction_east,
			direction_northwest,
			direction_northeast,
			direction_southwest,
			direction_southeast,

			max_value = direction_southeast
		};
	}
}