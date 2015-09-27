// name: key.h
// type: c++ header
// desc: system virtual key enum
// auth: is0urce

#pragma once

namespace px
{
	enum class key : int
	{
		not_valid,

		quick_save,
		quick_load,

		move_none,
		move_north,
		move_south,
		move_west,
		move_east,
		move_northwest,
		move_northeast,
		move_southwest,
		move_southeast,

		action_use,
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

		panel_inventory,
		panel_character,
		panel_skills,
		panel_craft,
		panel_options,

		command_ok,
		command_cancel,

		max_value = command_cancel
	};
}