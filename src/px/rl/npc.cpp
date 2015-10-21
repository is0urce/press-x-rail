// name: deposit.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "npc.h"

#include <px/game.h>
#include <px/scene.h>
#include <px/rl/unit.h>
#include <px/rl/player.h>

#include <px/astar.h>

namespace px
{
	namespace rl
	{
		namespace
		{
			const unsigned int alert_distance = 15;
		}
		enum class npc::ai_state : unsigned int
		{
			idle, alert
		};

		npc::npc() : m_ai_state(ai_state::idle)
		{
		}
		npc::~npc()
		{
		}

		unit::sign_t npc::signature()
		{
			return "npc";
		}

		unit::sign_t npc::sign_unit() const
		{
			return signature();
		}

		void npc::action_unit(environment &current)
		{
			person::action_unit(current);

			point dest = current.player()->position();

			if (m_ai_state == ai_state::idle)
			{
				if (current.distance(dest, position()) <= alert_distance || !health().full())
				{
					m_ai_state = ai_state::alert;
					current.broadcast({ "!", 0xff0000, position(), 1.0 });
				}
			}
			else if (m_ai_state == ai_state::alert)
			{
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
		}
	}
}