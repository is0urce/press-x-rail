// name: library.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#pragma once

#include "resource.h"

#include "npc.h"
#include "status.h"

#include <string>
#include <stdexcept>

namespace px
{
	class library :
		public resource<rl::npc>,
		public resource<rl::item>,
		public resource<rl::status<std::shared_ptr<rl::unit>>>
	{
	public:
		template <typename _T>
		_T prototype(std::string &key)
		{
			try
			{
				resource<_T> *rt = this;
				return rt->prototype(key);
			}
			catch (std::exception &e)
			{
				throw std::runtime_error(std::string("px::resource::prototype - ") + e.what() + " key=" + key);
			}
		}
		template <typename _T>
		void push(std::string tag, _T res)
		{
			try
			{
				resource<_T> *rt = this;
				rt->push(tag, res);
			}
			catch (std::exception &e)
			{
				throw std::runtime_error(std::string("px::resource::push - ") + e.what() + " key=" + tag);
			}
		}

		template <typename _T>
		_T prototype(const char *key)
		{
			return prototype<_T>(std::string(key));
		}
		template <typename _T>
		void push(const char *ctag, _T res)
		{
			push<_T>(std::string(ctag), res);
		}
	};
}