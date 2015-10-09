// name: io.h
// type: c++ header
// desc: class with implementation
// auth: is0urce

#ifndef PX_IO_H
#define PX_IO_H

#include <string>
#include <stdexcept>
#include <array>

namespace px
{
	class io
	{
	public:
		static const unsigned int key_size = 8;

	public:
		typedef std::array<char, key_size> key_t;
		typedef unsigned long chunk_size;

	public:
		io() {}
		virtual ~io() {}

	public:
		static key_t to_key(const std::string &str)
		{
			auto len = str.length();
			if (str.length() > key_size) throw std::logic_error("px::io::to_key(string) - length > key size, str=" + str);
			key_t result = {};
			for (unsigned int i = 0; i < len; ++i)
			{
				result[i] = str[i];
			}
			return result;
		}
		static key_t to_key(const char *c_str)
		{
			return to_key(std::string(c_str));
		}
		static key_t to_key(unsigned int key_value)
		{
			if (sizeof(key_value) > sizeof(key_t)) throw std::logic_error("px::io::to_key(uint) - not supported - int type size > key type size");

			key_t result;
			result.fill('_');
			memcpy((char*)&result, &key_value, sizeof(key_value));
			return result;
		}
		static std::string to_string(key_t key)
		{
			unsigned int len = 0;
			while (len < key_size && key[len]) ++len;
			return std::string(&key[0], len);
		}
	};
	inline bool operator==(const std::string &a, const io::key_t &b) { return operator==(io::to_key(a), b); }
	inline bool operator==(const io::key_t &a, const std::string &b) { return operator==(a, io::to_key(b)); }

	inline bool operator!=(const std::string &a, const io::key_t &b) { return !operator==(io::to_key(a), b); }
	inline bool operator!=(const io::key_t &a, const std::string &b) { return !operator==(a, io::to_key(b)); }

	inline bool operator==(const char *a, const io::key_t &b) { return operator==(io::to_key(a), b); }
	inline bool operator==(const io::key_t &a, const char *b) { return operator==(a, io::to_key(b)); }

	inline bool operator!=(const char *a, const io::key_t &b) { return !operator==(io::to_key(a), b); }
	inline bool operator!=(const io::key_t &a, const char *b) { return !operator==(a, io::to_key(b)); }
}

#endif