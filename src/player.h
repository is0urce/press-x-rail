// name: player.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "person.h"

namespace px
{
	class game;
	namespace rl
	{
		class player : public person
		{
		public:
			typedef game receiver_t;

		private:
			receiver_t &m_receiver;

			// ctor & dtor
		public:
			player(receiver_t &receiver);
			virtual ~player();

		protected:
			virtual void acquire(player::item_t item) override;
		};
	}
}