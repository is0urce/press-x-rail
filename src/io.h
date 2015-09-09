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
		typedef struct key_t
		{
			char letter[key_size];
		} key_t;
		typedef unsigned long chunk_size;

	public:
		io() {}
		virtual ~io() {}

	public:
		static key_t to_key(const std::string &str)
		{
			auto len = str.length();
			if (str.length() > key_size) throw std::logic_error("px::io::to_key(str) - length > key size, str=" + str);
			key_t result = {};
			for (unsigned int i = 0; i < len; ++i)
			{
				result.letter[i] = str[i];
			}
			return result;
		}
		static key_t to_key(unsigned int key_value)
		{
			if (sizeof(key_value) > sizeof(key_t)) throw std::logic_error("not supported");

			key_t result = to_key("_____key");
			memcpy((char*)&result, &key_value, sizeof(key_value));
			return result;
		}
	};
	inline bool operator==(const io::key_t &a, const io::key_t &b) { return std::equal(a.letter, a.letter + sizeof(a.letter) / sizeof (*a.letter), b.letter); }
	inline bool operator!=(const io::key_t &a, const io::key_t &b) { return !operator==(a,b); }
}
