// name: writer.h
// type: c++ header
// desc: class with implementation
// auth: is0urce

// chunk format writer
// chunk [name][data_size][data]
// name is char8, data_size is chunk_size_t, data size of data_size
// values written by write<_T>(key, value) in nodes, top node exposed by writer class
// data can be nested, open(key) for create nesting
// writing another value-size or opening another nested list at same level invalidates previous

#pragma once

#include "io.h"

#include <fstream>
#include <string>
#include <memory>
#include <stdexcept>

namespace px
{
	class writer : public io
	{
	public:
		class node;

	public:
		typedef std::ofstream stream_t;
		typedef std::shared_ptr<node> node_ptr;

	public:
		class node
		{
		private:
			stream_t *m_stream_ptr;
			node_ptr m_child; // current opened node
			bool m_opened; // is this node opened
			stream_t::streampos m_reserve;
			chunk_size m_acc;

		public:
			node(stream_t *stream_ptr) : m_acc(0), m_opened(true), m_stream_ptr(stream_ptr) {}
			~node() {}
		private:
			node(const node &n) = delete;

		public:
			void close()
			{
				if (m_child)
				{
					m_child->m_opened = false;
					m_child->close();

					auto current = m_stream_ptr->tellp();
					m_stream_ptr->seekp(m_reserve);
					m_stream_ptr->write((char*)&m_child->m_acc, sizeof(m_child->m_acc));
					m_stream_ptr->seekp(current);

					m_acc += sizeof(key_t) + sizeof(chunk_size) + m_child->m_acc;

					m_child.reset();
				}
			}
			node_ptr open(key_t key)
			{
				if (!m_opened) throw std::logic_error("writer::node::open() node closed");
				close();

				chunk_size reserved = 0;
				m_stream_ptr->write((char*)&key, sizeof(key));
				m_reserve = m_stream_ptr->tellp();
				if (m_reserve == stream_t::streampos(-1)) throw std::runtime_error("stream buffer does not support tellp(), or it fails");
				m_stream_ptr->write((char*)&reserved, sizeof(reserved));

				m_child.reset(new node(m_stream_ptr));
				return m_child;
			}
			void write(key_t key, const std::string &str)
			{
				if (!m_opened) throw std::logic_error("writer::node::write() node closed");
				close();

				chunk_size len = str.length();
				m_stream_ptr->write((char*)&key, sizeof(key));
				m_stream_ptr->write((char*)&len, sizeof(len));
				m_stream_ptr->write(str.c_str(), len);

				m_acc += sizeof(key) + sizeof(len) + len;
			}
			template <typename _E>
			void write(key_t key, _E element)
			{
				if (!m_opened) throw std::logic_error("writer::node::write() node closed");
				close();

				chunk_size element_size = sizeof(_E);
				m_stream_ptr->write((char*)&key, sizeof(key));
				m_stream_ptr->write((char*)&element_size, sizeof(element_size));
				m_stream_ptr->write((char*)&element, element_size);

				m_acc += sizeof(key) + sizeof(element_size) + element_size;
			}
			node_ptr open(const std::string &key)
			{
				return open(to_key(key));
			}
			template <typename _E>
			void write(const std::string &key, _E element)
			{
				write(to_key(key), element);
			}
			void write(const std::string &key, const char *cstr)
			{
				write(to_key(key), std::string(cstr));
			}
			void write(const std::string &key, const std::string &str)
			{
				write(to_key(key), str);
			}
			void write(key_t key, const char *cstr)
			{
				write(key, std::string(cstr));
			}

		};
	private:
		stream_t m_stream;
		node_ptr m_top;

	public:
		writer(const std::string &path) {
			init(path);
		}
		writer(stream_t stream) { init(); }
		virtual ~writer() { end(); }
	private:
		writer(const writer &w) = delete;

	private:
		void init(const std::string &path)
		{
			m_stream.open(path.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
			if (!m_stream.is_open())
			{
				throw std::runtime_error("can't write in file " + path);
			}
			init();
		}
		void init()
		{
			m_top.reset(new node(&m_stream));
		}

	public:
		node_ptr top() { return m_top; }
		void end() { m_top->close(); m_stream.flush(); }
		node_ptr operator->() { return m_top; }
	};
}