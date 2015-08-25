// name: item.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "unit.h"

namespace px
{
	namespace rl
	{
		class Actor;

		class item : public unit
		{
		public:
			item();
			virtual ~item();
		};
	}
}