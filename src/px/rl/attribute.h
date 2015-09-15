// name: attribute.h
// type: c++ header
// desc: enum
// auth: is0urce

// enum for statistics attributes

#ifndef PX_RL_ATTRIBUTE_H
#define PX_RL_ATTRIBUTE_H

namespace px
{
	namespace rl
	{
		enum class attribute : unsigned int
		{
			undefined,

			strength,
			intelligence,
			dexterity,

			vitality,

			damage,
			armor,

			dodge_bonus,
			resist_bonus,
			critical_bonus,
			accuracy_bonus,

			min_attribute = undefined,
			max_attribute = accuracy_bonus
		};

	}
}

#endif