// name: door.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_DOOR_H
#define PX_RL_DOOR_H

#include <px/rl/unit.h>

namespace px
{
	namespace rl
	{
		class door : public unit
		{
		private:
			bool m_open;
			appearance_t m_opened_appearance;
			appearance_t m_closed_appearance;

			// ctor & dtor
		public:
			door(bool state) : m_open(state) {}
			door() : m_open(false) {}
			virtual ~door() {}

			// vitrual
		protected:
			virtual bool traversable_unit() const override { return m_open; }
			virtual bool transparent_unit() const override { return m_open; }
			virtual bool useable_unit(user_t user) const override { return !m_open; }
			virtual void use_unit(user_t user) override { if (!m_open) open(); }
			virtual void serialize(writer::node_ptr node) const override
			{
				auto l = node->open("door");
				l->write("open", m_open);
				l->write("opn_app", m_opened_appearance);
				l->write("cls_app", m_closed_appearance);
				unit::serialize(l);
			}
			virtual void deserialize(const reader::node &node) override
			{
				node["open"] >> m_open;
				node["opn_app"] >> m_opened_appearance;
				node["cls_app"] >> m_closed_appearance;
				unit::deserialize(node["unit"]);
			}

		public:
			void open()
			{
				m_open = true;
				entity::appearance(m_opened_appearance);
			}
			void close()
			{
				m_open = false;
				entity::appearance(m_closed_appearance);
			}
			bool opened() const
			{
				return m_open;
			}
			void opened(bool is_opened)
			{
				if (is_opened)
				{
					open();
				}
				else
				{
					close();
				}
			}
			void appearance(appearance_t opened_appearance, appearance_t closed_appearance)
			{
				m_opened_appearance = opened_appearance;
				m_closed_appearance = closed_appearance;
				opened(m_open); // update
			}
		};
	}
}

#endif