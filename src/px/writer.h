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
#include <px/writer_node.h>

#include <fstream>
#include <string>
#include <memory>
#include <stdexcept>

namespace px
{
	class writer : public io
	{
	public:
		typedef writer_node node;
		typedef writer_node::stream_t stream_t;
		typedef node::ptr node_ptr;

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