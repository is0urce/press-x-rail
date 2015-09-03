// name: resource.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include <map>
#include <string>

namespace px
{
	template <typename _R>
	class resource
	{
	private:
		std::map<std::string, _R> m_resources;

	public:
		void push(std::string tag, _R resource)
		{
			m_resources.emplace(tag, resource);
		}
		_R prototype(std::string &tag)
		{
			return m_resources.at(tag);
		}
	};
}