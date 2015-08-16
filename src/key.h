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
			not_valid,
			action0,
			action1,
			action2,
			action3,
			action4,
			action5,
			action6,
			action7,
			action8,
			action9,
			command_ok,
			command_cancel,
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