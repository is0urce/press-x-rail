// name: effect.h
// type: c++ header
// desc: declaration of interface
// auth: is0urce

// forms visitor pattern with conjunction with unit.apply virtual method

#ifndef PX_RL_EFFECT_H
#define PX_RL_EFFECT_H

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
			void apply(character &u) { apply_effect(u); }
		};
	}
}

#endif