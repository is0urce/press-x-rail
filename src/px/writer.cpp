// name: writer.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "writer.h"

namespace px
{
	writer::writer(const std::string &path)
	{
		m_stream.open(path.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
		if (!m_stream.is_open())
		{
			throw std::runtime_error("can't open file " + path);
		}
		if (!m_stream)
		{
			throw std::runtime_error("stream error in file " + path);
		}
		m_top.reset(new node(&m_stream));
	}
	writer::~writer()
	{
		end();
	}

	writer::node_ptr writer::top() { return m_top; }
	void writer::end() { m_top->close(); m_stream.flush(); }
	writer::node_ptr writer::operator->() { return m_top; }
}
