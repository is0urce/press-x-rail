// name: reader.h
// type: c++ header
// desc: class with implementation
// auth: is0urce

#ifndef PX_READER_H
#define PX_READER_H

#include <px/io.h>
#include <px/reader_node.hpp>

#include <fstream>

namespace px
{
	class reader : public io
	{
	public:
		typedef reader_node node;
		typedef node::stream stream;

	private:
		stream m_stream;
		node m_top;

	public:
		reader(const std::string &path);
		virtual ~reader();

	public:
		node operator[](const std::string &key) const;
	};
}

#endif