// name: writer_node.h
// type: c++ header
// desc: class with implementation
// auth: is0urce

#ifndef PX_READER_NODE_H
#define PX_READER_NODE_H

#include <px/io.h>

#include <fstream>
#include <string>
#include <memory>
#include <stdexcept>
#include <vector>
#include <functional>

namespace px
{
	namespace
	{
		const unsigned int header_size = sizeof(io::key_t) + sizeof(io::chunk_size);
	}
	class reader_node
	{
	public:
		typedef std::ifstream stream;

	private:
		stream *m_stream_ptr;
		stream::streampos m_pos;
		stream::streampos m_out;
		io::key_t m_key;
		io::chunk_size m_size;

	public:
		reader_node(stream* stream_ptr) : m_stream_ptr(stream_ptr) {}
		reader_node(stream* stream_ptr, stream::streampos pos, stream::streampos pos_out) : m_stream_ptr(stream_ptr), m_pos(pos), m_out(pos_out)
		{
			read_header();
		}
		~reader_node()
		{
		}

	private:
		void focus() const
		{
			m_stream_ptr->seekg(m_pos);
		}
		void read_header() const
		{
			focus();
			m_stream_ptr->read((char*)&m_key, sizeof(m_key));
			m_stream_ptr->read((char*)&m_size, sizeof(m_size));
		}
	public:
		void init(stream::streampos pos, stream::streampos pos_out)
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
		reader_node next() const
		{
			stream::streampos pos = m_pos;
			pos += header_size + m_size;
			return reader_node(m_stream_ptr, pos, m_out);
		}
		io::key_t name() const
		{
			return m_key;
		}
		io::chunk_size size() const
		{
			return m_size;
		}
		void read(char* destination, io::chunk_size size) const
		{
			if (size != m_size) throw std::runtime_error("px::reader::node::read<_E>() - type size mismatch");
			read_header();
			m_stream_ptr->read(destination, size);
		}
		std::string& read(std::string &str) const
		{
			read_header();
			unsigned int size = (unsigned int)m_size;
			if (size > 0)
			{
				std::vector<char> buf(size);
				m_stream_ptr->read(&buf[0], size);
				str.assign(&buf[0], size);
			}
			else
			{
				str = "";
			}
			return str;
		}
		template <typename _E>
		_E& read(_E &element) const
		{
			read_header();
			if (sizeof(element) != m_size) throw std::runtime_error("px::reader::node::read<_E>() - type size mismatch");
			m_stream_ptr->read((char*)&element, sizeof(element));
			return element;
		}
		template <typename _E>
		const reader_node& operator>>(_E &element) const
		{
			read(element);
			return *this;
		}
		const reader_node& operator>>(std::string &str) const
		{
			read(str);
			return *this;
		}
		std::string read() const
		{
			std::string str;
			read(str);
			return str;
		}
		template <typename _E>
		_E read() const
		{
			_E element;
			read(element);
			return element;
		}
		reader_node operator[](const io::key_t &key) const
		{
			auto pos = m_pos;
			auto pos_out = m_pos;
			pos += header_size;
			pos_out += header_size + m_size;

			reader_node caret(m_stream_ptr, pos, pos_out);

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
		void enumerate(std::function<void(reader_node)> enum_fn) const
		{
			if (m_size == 0) return; // no nodes

			auto pos = m_pos;
			auto pos_out = m_pos;
			pos += header_size;
			pos_out += header_size + m_size;

			reader_node caret(m_stream_ptr, pos, pos_out);

			enum_fn(caret);
			while (!caret.last())
			{
				caret = caret.next();
				enum_fn(caret);
			}
		}
		reader_node operator[](const std::string &key_name) const
		{
			try
			{
				return operator[](io::to_key(key_name));
			}
			catch (std::logic_error &exc)
			{
				throw std::logic_error(exc.what() + std::string(" key=") + key_name);
			}
		}
	};
}

#endif