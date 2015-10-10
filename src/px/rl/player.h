// name: player.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_PLAYER_H
#define PX_RL_PLAYER_H

#include <px/rl/person.h>

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
			receiver_t *m_receiver;

		public:
			static std::string signature();

			// ctor & dtor
		public:
			player(receiver_t *receiver);
			virtual ~player();

		protected:
			virtual void acquire(player::item_t item, bool silent) override;
			virtual std::string sign_unit() const override;
		};
	}
}

#endif