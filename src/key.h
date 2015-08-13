// name: key.h
// type: c++ header
// desc: system virtual key enum
// auth: is0urce

#pragma once

namespace px
{
	namespace shell
	{
		enum class key : int
		{
			move_none,
			move_north,
			move_south,
			move_west,
			move_east,
			move_northwest,
			move_northeast,
			move_southwest,
			move_southeast,

			max_value = move_southeast
		};
	}
}