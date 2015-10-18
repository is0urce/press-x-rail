// name: reader.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "reader.h"

#include <string>
#include <stdexcept>

namespace px
{
	reader::reader(const std::string &path) : m_top(&m_stream)
	{
		m_stream.open(path.c_str(), stream::in | stream::binary);
		if (!m_stream.is_open())
		{
			throw std::runtime_error("px::reader::reader(..) - can't read from file, path=" + path);
		}
		auto start = m_stream.tellg();
		m_stream.seekg(0, m_stream.end);
		auto end = m_stream.tellg();
		if (start == end)
		{
			throw std::runtime_error("px::reader::reader(..) - file is empty, path=" + path);
		}
		m_stream.seekg(start);
		m_top.init(start, end);
	}
	reader::~reader()
	{
	}

	reader::node reader::operator[](const std::string &key) const
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
}