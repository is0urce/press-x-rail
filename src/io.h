// name: io.h
// type: c++ header
// desc: class with implementation
// auth: is0urce

#pragma once

#include <string>
#include <stdexcept>

namespace px
{
	class io
	{
	public:
		static const unsigned int key_size = 8;

	public:
		typedef struct key_t { char letter[key_size]; } key_t;
		typedef uint64_t chunk_size;

	public:
		static key_t to_key(const std::string &str)
		{
			auto len = str.length();
			if (str.length() > key_size) throw std::logic_error("px::io - to_key(&str) - length > key size");
			key_t result = {};
			for (unsigned int i = 0; i < len; ++i)
			{
				result.letter[i] = str[i];
			}
			return result;
		}

	public:
		io() {}
		virtual ~io() {}
	};
}
