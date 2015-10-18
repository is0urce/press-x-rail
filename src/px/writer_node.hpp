// name: writer_node.h
// type: c++ header
// desc: class with implementation
// auth: is0urce

// node of writer class
// node can be opened or closed
// opened node is container currently in processing
// closed node is filled container or value plain data created in already closed state
// you can not write in closed node, only open another node in parent still opened nodes
// top node created as open container

#ifndef PX_WRITER_NODE_H
#define PX_WRITER_NODE_H

#include <px/io.h>

#include <fstream>
#include <string>
#include <memory>
#include <stdexcept>

namespace px
{
	class writer_node : public io
	{
	public:
		typedef std::ofstream stream_t;
		typedef std::shared_ptr<writer_node> ptr;

	private:
		stream_t *m_stream_ptr;
		ptr m_child; // current opened node
		bool m_opened; // is this node opened
		stream_t::streampos m_reserve;
		chunk_size m_acc;

	public:
		writer_node(stream_t *stream_ptr) : m_acc(0), m_opened(true), m_stream_ptr(stream_ptr)
		{
			if (!stream_ptr) throw std::logic_error("px::writer_node::ctor(stream) stream is null");
		}
		~writer_node()
		{
		}
		writer_node(const writer_node&) = delete;

	public:
		// close current opened list
		void close()
		{
			if (m_child)
			{
				m_child->m_opened = false;
				m_child->close();

				auto current = m_stream_ptr->tellp();
				m_stream_ptr->seekp(m_reserve);
				m_stream_ptr->write((char*)&m_child->m_acc, sizeof(chunk_size));
				m_stream_ptr->seekp(current);

				m_acc += sizeof(key_t) + sizeof(chunk_size) + m_child->m_acc;

				m_child.reset();
			}
		}
		ptr open(key_t key)
		{
			if (!m_opened) throw std::logic_error("writer::node::open() node closed");
			close();
			if (m_stream_ptr->fail()) throw std::runtime_error("px::writer::open() - stream not invalid");

			chunk_size reserved;
			memset(&reserved, 0, sizeof(reserved));

			m_stream_ptr->write((char*)&key, sizeof(key));

			m_reserve = m_stream_ptr->tellp(); // reserve and save position for list size
			if (m_reserve == stream_t::streampos(-1)) throw std::runtime_error("stream buffer does not support tellp(), or it fails");
			m_stream_ptr->write((char*)&reserved, sizeof(chunk_size));

			if (m_stream_ptr->fail()) throw std::runtime_error("px::writer::open() - operation fails");

			m_child = std::make_shared<writer_node>(m_stream_ptr);
			return m_child;
		}
		void write(key_t key, const std::string &str)
		{
			if (!m_opened) throw std::logic_error("writer::node::write() node closed");
			close();

			chunk_size len = str.length();
			m_stream_ptr->write((char*)&key, sizeof(key));
			m_stream_ptr->write((char*)&len, sizeof(len));
			if (len > 0)
			{
				m_stream_ptr->write(str.c_str(), len);
			}

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
		void write(key_t key, const char* source, chunk_size size)
		{
			if (!m_opened) throw std::logic_error("writer::node::write() node closed");
			if (!source) throw std::logic_error("px::writer_node::write(key, source, size) source is null");

			close();

			chunk_size element_size = size;
			m_stream_ptr->write((char*)&key, sizeof(key));
			m_stream_ptr->write((char*)&element_size, sizeof(element_size));
			m_stream_ptr->write(source, size);

			m_acc += sizeof(key) + sizeof(element_size) + element_size;
		}
		ptr open(const std::string &key)
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
		void write(key_t key, const char *c_str)
		{
			write(key, std::string(c_str));
		}
		void write(const std::string &key, const char* source, chunk_size size)
		{
			write(to_key(key), source, size);
		}
	};
}

#endif