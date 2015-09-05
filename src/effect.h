// name: effect.h
// type: c++ header
// desc: declaration of interface
// auth: is0urce

// forms visitor pattern with conjunction with unit.apply virtual method

#pragma once

namespace px
{
	namespace rl
	{
		class character;
		class effect
		{
			// ctor & dtor
		public:
			virtual ~effect() = 0;

			// virtual methods
		protected:
			virtual void apply_effect(character &mobile) = 0;

			// methods
		public:
			void apply(character &mobile) { apply_effect(mobile); }
		};
	}
}