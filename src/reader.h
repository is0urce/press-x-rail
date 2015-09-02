// name: reader.h
// type: c++ header
// desc: class with implementation
// auth: is0urce

#pragma once

#include "io.h"

#include <fstream>
#include <functional>
#include <string>
#include <memory>
#include <stdexcept>

namespace px
{
	class reader : public io
	{
	public:
		static const unsigned int header_size = sizeof(key_t) + sizeof(chunk_size);

	public:
		typedef std::ifstream stream_t;

	public:
		class node
		{
		private:
			stream_t *m_stream_ptr;
			stream_t::streampos m_pos;
			stream_t::streampos m_out;
			key_t m_key;
			chunk_size m_size;

		public:
			node(stream_t* stream_ptr) : m_stream_ptr(stream_ptr) {}
			node(stream_t* stream_ptr, stream_t::streampos pos, stream_t::streampos pos_out) : m_stream_ptr(stream_ptr), m_pos(pos), m_out(pos_out)
			{
				read_header();
			}
			~node() {}

		private:
			void focus()
			{
				m_stream_ptr->seekg(m_pos);
			}
			void read_header()
			{
				focus();
				m_stream_ptr->read((char*)&m_key, sizeof(m_key));
				m_stream_ptr->read((char*)&m_size, sizeof(m_size));
			}
		public:
			void init(stream_t::streampos pos, stream_t::streampos pos_out)
			{
				m_pos = pos;
				m_out = pos_out;
				read_header();
			}
			bool last() const
			{
				auto next_pos = m_pos;
				next_pos += header_size + m_size;
				return next_pos >= m_out;
			}
			node next() const
			{
				stream_t::streampos pos = m_pos;
				pos += header_size + m_size;
				return node(m_stream_ptr, pos, m_out);
			}
			key_t name() const
			{
				return m_key;
			}
			chunk_size size() const
			{
				return m_size;
			}
			std::string& read(std::string &str)
			{
				read_header();
				unsigned int size = (unsigned int)m_size;
				std::vector<char> buf(size);
				m_stream_ptr->read(&buf[0], size);
				str.assign(&buf[0], size);
				return str;
			}
			template <typename _E>
			_E& read(_E &element)
			{
				read_header();
				if (sizeof(element) != m_size) throw std::runtime_error("px::reader::node::read<_E>() - type size mismatch");
				m_stream_ptr->read((char*)&element, sizeof(element));
				return element;
			}
			template <typename _E>
			node& operator>>(_E &element)
			{
				read(element);
				return *this;
			}
			node& operator>>(std::string &str)
			{
				read(str);
				return *this;
			}
			std::string read()
			{
				std::string str;
				read(str);
				return str;
			}
			template <typename _E>
			_E read()
			{
				_E element;
				read(element);
				return element;
			}
			node operator[](const key_t &key)
			{
				auto pos = m_pos;
				auto pos_out = m_pos;
				pos += header_size;
				pos_out += header_size + m_size;

				node caret(m_stream_ptr, pos, pos_out);

				if (key == caret.name())
				{
					return caret;
				}
				while (!caret.last())
				{
					caret = caret.next();
					if (key == caret.name())
					{
						return caret;
					}
				}

				throw std::logic_error("px::reader::node operator[](..) key not exists");
			}
			void enumerate(std::function<void(node)> fn)
			{
				auto pos = m_pos;
				auto pos_out = m_pos;
				pos += header_size;
				pos_out += header_size + m_size;

				node caret(m_stream_ptr, pos, pos_out);

				fn(caret);
				while (!caret.last())
				{
					caret = caret.next();
					fn(caret);
				}
			}
			node operator[](const std::string &key_name)
			{
				try
				{
					return operator[](to_key(key_name));
				}
				catch (std::logic_error &exc)
				{
					throw std::logic_error(exc.what() + std::string(" key=") + key_name);
				}
			}
		};

	private:
		stream_t m_stream;
		node m_top;

	public:
		reader(const std::string &path) : m_top(&m_stream)
		{
			m_stream.open(path.c_str(), std::ios::in | std::ios::binary);
			if (!m_stream.is_open())
			{
				throw std::runtime_error("can't read from file " + path);
			}
			auto start = m_stream.tellg();
			m_stream.seekg(0, m_stream.end);
			auto end = m_stream.tellg();
			m_stream.seekg(start);
			m_top.init(start, end);
		}
		virtual ~reader() {}

	public:
		node operator[](const std::string &key)
		{
			key_t name = to_key(key);
			node caret = m_top;
			while (!m_stream.eof())
			{
				if (name == caret.name())
				{
					return caret;
				}
				caret = caret.next();
			}
			throw std::logic_error("px::reader operator[] key not exists, key=" + key);
		}
	};
}