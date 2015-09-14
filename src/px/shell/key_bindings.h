// name: key_bindings.h
// type: c++ header
// desc: class to translate system virtual key messages to shell virtual key messages
// auth: is0urce

#pragma once

#include <map>

namespace px
{
	namespace shell
	{
		template <typename _SK, typename _VK>
		class key_bindings
		{
		private:
			std::map<_SK, _VK> m_bindings;
		public:
			key_bindings() {}
			~key_bindings() {}

			void bind(_SK key, _VK vkey) { m_bindings[key] = vkey; }
			void bind(_SK key1, _SK key2, _VK vkey) { bind(key1, vkey); bind(key2, vkey); }
			void bind(_SK key1, _SK key2, _SK key3, _VK vkey) { bind(key1, vkey); bind(key2, vkey); bind(key3, vkey); }
			bool find(_SK key, _VK& vkey) const
			{
				auto i = m_bindings.find(key);
				if (i != m_bindings.end())
				{
					vkey = i->second;
					return true;
				}
				return false;
			}
			_VK operator[](_SK key) const { return m_bindings.at(key); }
		};
	}
}
