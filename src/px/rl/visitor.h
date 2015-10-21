// name: visitor.h
// type: c++ header
// desc: declaration of interface
// auth: is0urce

// forms visitor pattern using unit.accept virtual method

#ifndef PX_RL_VISITOR_H
#define PX_RL_VISITOR_H

namespace px
{
	namespace rl
	{
		class person;
		class visitor
		{
			// ctor & dtor
		public:
			virtual ~visitor() {};

			// virtual methods
		protected:
			virtual void expose_person(person &mobile) {};

			// methods
		public:
			void expose(person &mobile)
			{ 
				expose_person(mobile);
			}
		};
	}
}

#endif