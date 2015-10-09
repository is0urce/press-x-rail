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

#ifndef PX_WRITER_H
#define PX_WRITER_H

#include <px/io.h>

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
			node_ptr open(key_t key)
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
			void write(key_t key, const char* src, chunk_size size)
			{
				if (!m_opened) throw std::logic_error("writer::node::write() node closed");
				close();

				chunk_size element_size = size;
				m_stream_ptr->write((char*)&key, sizeof(key));
				m_stream_ptr->write((char*)&element_size, sizeof(element_size));
				m_stream_ptr->write(src, size);

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
			void write(const std::string &key, const char* src, chunk_size size)
			{
				write(to_key(key), src, size);
			}

		};
	private:
		stream_t m_stream;
		node_ptr m_top;

	public:
		writer(const std::string &path);
		virtual ~writer();
		writer(const writer &w) = delete;

	public:
		node_ptr top();
		void end();
		node_ptr operator->();
	};
}

#endif