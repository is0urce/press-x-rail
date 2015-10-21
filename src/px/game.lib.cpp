// name: game.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "game.h"

#include <px/library.h>
#include <px/rl/effect.h>

namespace px
{
	void game::generate_library()
	{
		m_library = std::make_shared<library>();

		// buffs
		rl::person::status_t poison(10, [&](rl::person &c, rl::person::status_t &s) { c.health() = c.health() - 1; });
		poison.register_apply([&](rl::person &target, rl::person::status_t &s)
		{
			m_broadcasts.emplace_back("* poisoned *", color(0, 1, 0), target.position(), 0.5);
		});
		poison.appearance({ '-', 0xffff00 });
		poison.name("Poison");
		poison.tag("poison");
		m_library->insert(poison);

		// skills

		rl::person::action_t::range_t bite_range(0, 1);
		rl::person::action_t bite(rl::person::action_t::target_fn([&](rl::person::caster_t *user, rl::person::target_t unit)
		{
			if (user && unit)
			{
				point start = user->position();

				// status
				unit->accept(rl::effect(m_library->prototype<rl::person::status_t>("poison")));

				// popup
				m_broadcasts.emplace_back(std::to_string(-8), color(1, 0, 0), unit->position(), 0.5);

				// visual
				m_projectiles.push_back(projectile({ '>', 0xff0000 }, {},
					[start, unit](projectile::timespan_t phase)
				{
					point fin = unit->position();
					vector pos = fin + vector((fin - start).normalized()) * -0.25;
					vector up = pos + vector(0, 0.25);
					return up.lerp(pos, (std::min)(phase, 1.0));
				}, 3.14 * -0.5, 0.25));
				m_projectiles.push_back(projectile({ '>', 0xff0000 }, {},
					[start, unit](projectile::timespan_t phase)
				{
					point fin = unit->position();
					vector pos = fin + vector((fin - start).normalized()) * -0.25 + vector(0.15, 0);
					vector up = pos + vector(0, 0.25);
					return up.lerp(pos, (std::min)(phase, 1.0));
				}, 3.14 * -0.5, 0.25));
				m_projectiles.push_back(projectile({ '>', 0xff0000 }, {},
					[start, unit](projectile::timespan_t phase)
				{
					point fin = unit->position();
					vector pos = fin + vector((fin - start).normalized()) * -0.25 - vector(0.15, 0);
					vector up = pos + vector(0, 0.25);
					return up.lerp(pos, (std::min)(phase, 1.0));
				}, 3.14 * -0.5, 0.25));
				m_projectiles.push_back(projectile({ '>', 0xff0000 }, {},
					[start, unit](projectile::timespan_t phase)
				{
					point fin = unit->position();
					vector pos = fin + vector((fin - start).normalized()) * -0.25;
					vector down = pos - vector(0, 0.25);
					return down.lerp(pos, (std::min)(phase, 1.0));
				}, 3.14 * 0.5, 0.25));
				m_projectiles.push_back(projectile({ '>', 0xff0000 }, {},
					[start, unit](projectile::timespan_t phase)
				{
					point fin = unit->position();
					vector pos = fin + vector((fin - start).normalized()) * -0.25 + vector(0.15, 0);
					vector down = pos - vector(0, 0.25);
					return down.lerp(pos, (std::min)(phase, 1.0));
				}, 3.14 * 0.5, 0.25));
				m_projectiles.push_back(projectile({ '>', 0xff0000 }, {},
					[start, unit](projectile::timespan_t phase)
				{
					point fin = unit->position();
					vector pos = fin + vector((fin - start).normalized()) * -0.25 - vector(0.15, 0);
					vector down = pos - vector(0, 0.25);
					return down.lerp(pos, (std::min)(phase, 1.0));
				}, 3.14 * 0.5, 0.25));
			}
		}), rl::person::action_t::target_check_fn([&, bite_range](rl::person::caster_t *user, rl::person::target_t unit)
		{
			return user && unit && user != unit.get() && rl::person::action_t::in_range(bite_range, distance(user->position(), unit->position()));
		}));
		bite.name("Bite");
		bite.range(bite_range);
		bite.tag("bite");
		m_library->insert(bite);

		rl::person::action_t pyroblast(rl::person::action_t::target_fn([&](rl::person::caster_t *user, rl::person::target_t unit)
		{
			if (user)
			{
				//broadcast({ "puff!", 0xffffff, user->position() });
			}
			if (user && unit)
			{
				vector start = user->position();
				vector fin = unit->position();
				//m_projectiles.push_back(projectile('*', 0xff0000, color(30, 10, 10), [=](projectile::timespan_t phase) { return start.lerp(fin, (std::min)(phase, 1.0)); }));
			}
		}), rl::person::action_t::target_check_fn([&](rl::person::caster_t *user, rl::person::target_t unit)
		{
			return true;
		}));
		pyroblast.tag("pyroblast");
		m_library->insert(pyroblast);

		// heal
		rl::person::action_t::ground_fn h([&](rl::person::caster_t *user, const point &position)
		{
			user->health() = user->health() + 1;
		});
		rl::person::action_t::ground_check_fn hc([&](rl::person::caster_t *user, const point &position)
		{
			return true;
		});
		rl::person::action_t heal(h, hc);
		heal.tag("heal");
		m_library->insert(heal);

		// teleport
		rl::person::action_t::ground_fn teleport_fn([&](rl::person::caster_t *user, const point &position)
		{
			m_scene->focus(position);
			m_scene->move(m_player, position);
		});
		rl::person::action_t::ground_check_fn teleport_check_fn([&](rl::person::caster_t *user, const point &position)
		{
			return true;
		});
		rl::person::action_t teleport(teleport_fn, teleport_check_fn);
		teleport.tag("teleport");
		m_library->insert(teleport);

		// npc

		rl::npc rat;
		rat.fraction(0);
		rat.appearance({ 'r', 0xff0000, 0.62f });
		rat.health() = 100;
		rat.tag("mob_rat");
		rat.name("Rat");
		rat.add_skill(bite);
		m_library->insert(rat);

		rl::item ore;
		ore.appearance('o');
		ore.name("copper ore");
		ore.tag("ore_copper");
		ore.stackable(true);
		m_library->insert(ore);

		rl::unit lantern;
		lantern.appearance({ ' ', color(1, 1, 1) });
		lantern.light({ { 3, 3, 3 }, true, true });
		lantern.tag("lantern");
		m_library->insert(lantern);

		rl::door door;
		door.appearance({ ' ', 0x333333 }, { '+', 0x333333 });
		door.tag("door");
		m_library->insert(door);


		rl::item ff;
		ff.name("Gribus Ogmelisius");
		ff.tag("ff");
		m_library->insert(ff);

		rl::deposit f1(m_library->make<rl::item>("ff"));
		f1.tag("fireflower");
		f1.name("Gribus Ognelisius");
		f1.appearance({ '*', color(1, 0, 0), 0.75 });
		f1.light({ { 3, 0, 0 }, true, true });
		f1.invincible(true);
		m_library->insert(f1);

		rl::deposit f2(std::make_shared<rl::item>());
		f2.tag("iceflower");
		f2.appearance({ '*', color(0, 0, 1), 0.75 });
		f2.light({ { 0, 0, 3 }, true, true });
		f2.invincible(true);
		m_library->insert(f2);

		rl::deposit flower(std::make_shared<rl::item>());
		flower.tag("felflower");
		flower.appearance({ '*', color(0, 1, 0), 0.75 });
		flower.light({ { 0, 3, 0 }, true, true });
		flower.invincible(true);
		m_library->insert(flower);
	}
}