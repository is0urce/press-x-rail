// name: writer.h
// type: c++ header
// desc: class with implementation
// auth: is0urce

// 2-d point with real components

#pragma once

#include "coordinate.h"
#include "point.h"

#include <fstream>
#include <string>
#include <memory>

namespace px
{
	template <typename _I, typename _F>
	class writer
	{
		class node;
	public:
		typedef std::ofstream stream_t;
		typedef uint64_t key_t;
		typedef uint64_t chunk_size;
		typedef std::shared_ptr<node> node_ptr;

	public:
		class node
		{
		private:
			stream_t *m_stream;
			node_ptr m_child;
			unsigned int m_acc_size;

		public:
			node(stream_t *stream) { m_stream = stream; }
			~node() {}

		public:
			template <typename _E>
			void write(key_t key, _E element)
			{
				chunk_size element_size = sizeof(_E);
				m_stream->write((char*)&key, sizeof(key));
				m_stream->write((char*)&element_size, sizeof(element_size));
				m_stream->write((char*)&element, element_size);
			}
			void write(key_t key, const std::string &str)
			{
				chunk_size len = str.length();
				m_stream->write((char*)&key, sizeof(key));
				m_stream->write((char*)&len, sizeof(len));
				m_stream->write(str.c_str(), len);
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
		~writer() { m_stream.flush(); }

	private:
		void init(const std::string &path)
		{
			m_stream.open(path.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
			if (!m_stream.is_open())
			{
				throw std::runtime_error("can't write in file" + path);
			}
			init();
		}
		void init()
		{
			m_top.reset(new node(&m_stream));
		}

	public:
		node_ptr top() { return m_top; }
	};
}
