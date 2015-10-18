// name: door.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_DOOR_H
#define PX_RL_DOOR_H

#include <px/rl/unit.h>

#include <px/writer_node.hpp>
#include <px/reader_node.hpp>

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

			// static
		public:
			static sign_t signature()
			{
				return "door";
			}

			// vitrual
		protected:
			virtual bool traversable_unit() const override { return m_open; }
			virtual bool transparent_unit() const override { return m_open; }
			virtual bool useable_unit(const environment&, user_t user) const override { return !m_open; }
			virtual void use_unit(environment&, user_t user) override { if (!m_open) open(); }
			virtual sign_t sign_unit() const override { return signature(); }
			virtual void serialize(o_node node, const serializer &s) const override
			{
				node->write("open", m_open);
				node->write("opn_app", m_opened_appearance);
				node->write("cls_app", m_closed_appearance);
				unit::serialize(node, s);
			}
			virtual void deserialize(const i_node &node, const serializer &s) override
			{
				node["open"] >> m_open;
				node["opn_app"] >> m_opened_appearance;
				node["cls_app"] >> m_closed_appearance;
				unit::deserialize(node, s);
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