// name: container.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_CONTAINER_H
#define PX_RL_CONTAINER_H

#include <px/rl/unit.h>
#include <px/rl/item.h>
#include <px/rl/inventory.h>
#include <px/ui/main_panel.h>
#include <px/ui/container_panel.h>

namespace px
{
	namespace rl
	{
		class container : public unit, public inventory<std::shared_ptr<item>>
		{
		public:
			typedef std::weak_ptr<unit> player_t;
			typedef std::weak_ptr<ui::main_panel> panel_t;

		private:
			player_t m_player;
			panel_t m_panel;

			// ctor & dtor
		public:
			container(player_t unit, panel_t ui_panel) : m_player(unit), m_panel(ui_panel)
			{
			}
			virtual ~container() {}

			// vitrual
		protected:
			virtual void use_unit(user_t user) override 
			{
				auto player = m_player.lock();
				auto stack = m_panel.lock();
				if (stack && user == player)
				{
					stack->add("container", std::make_shared<ui::container_panel>(user, this, stack->ui()));
					stack->open_container();
				}
			}
			virtual bool useable_unit(user_t user) const override { return user == m_player.lock(); }
		};
	}
}

#endif