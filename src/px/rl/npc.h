// name: npc.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_NPC_H
#define PX_RL_NPC_H

#include <px/rl/person.h>

#include <px/game.h>
#include <px/scene.h>
#include <px/rl/unit.h>
#include <px/rl/player.h>

#include <px/astar.h>

namespace px
{
	namespace rl
	{
		class npc : public person
		{
		private:
			unsigned int m_ai_state;

			// ctor & dtor
		public:
			npc() : m_ai_state(0)
			{
			}
			virtual ~npc()
			{
			}

		public:
			static sign_t signature() { return "npc"; }

		private:
			virtual sign_t sign_unit() const override { return signature(); }
			virtual void action_unit(environment &current) override
			{
				point dest = current.player()->position();
				auto scene = current.scene();
				auto path = path::find(position(), dest, 50, [&](const point &p) { return scene->traversable(p); });
				if (path)
				{
					auto step = path->begin();
					if (step != path->end())
					{
						auto blocking = scene->blocking(*step);
						if (blocking && !blocking->invincible())
						{
							for (auto &skill : m_skills)
							{
								if (skill.targeted() && skill.useable(this, blocking))
								{
									skill.use(this, blocking);
									break;
								}
								else if (skill.useable(this, dest))
								{
									skill.use(this, dest);
									break;
								}
							}
						}
						else
						{
							scene->move(*this, *step);
						}
					}
				}
			}
		};
	}
}

#endif