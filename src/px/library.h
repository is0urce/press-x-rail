// name: library.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_LIBRARY_H
#define PX_LIBRARY_H

#include "resource.h"

#include <px/rl/npc.h>
#include <px/rl/deposit.h>
#include <px/rl/door.h>
#include <px/rl/container.h>

#include <string>
#include <stdexcept>
#include <memory>

namespace px
{
	class library :
		public resource<rl::person::status_t>,
		public resource<rl::person::action_t>,
		public resource<rl::deposit>,
		public resource<rl::unit>,
		public resource<rl::npc>,
		public resource<rl::item>,
		public resource<rl::door>,
		public resource<rl::container>
	{
	public:
		template <typename _T>
		_T prototype(const std::string &key) const
		{
			try
			{
				const resource<_T> *rt = this;
				if (!rt) throw std::logic_error("px::library::prototype<T>(string key) - can't contain this type (i.e. not having T subtype)");
				return rt->prototype(key);
			}
			catch (std::exception &e)
			{
				throw std::runtime_error(std::string("px::library::prototype<T>(string key) - ") + e.what() + " key=" + key);
			}
		}
		template <typename _T>
		void insert(std::string tag, _T res)
		{
			try
			{
				resource<_T> *rt = this;
				if (!rt) throw std::logic_error("px::library::insert<T>(string key, T) - can't contain this type (i.e. not having T subtype)");
				rt->insert(tag, res);
			}
			catch (std::exception &e)
			{
				throw std::runtime_error(std::string("px::library::insert<T>(string key, T) - ") + e.what() + " key=" + tag);
			}
		}

		template <typename _T>
		_T prototype(const char *key) const
		{
			return prototype<_T>(std::string(key));
		}
		template <typename _T>
		void insert(const char *ctag, _T res)
		{
			insert<_T>(std::string(ctag), res);
		}
		template <typename _T>
		void insert(_T res)
		{
			insert<_T>(res.tag(), res);
		}
		template <typename _T>
		std::shared_ptr<_T> make(const std::string &key) const
		{
			return std::make_shared<_T>(prototype<_T>(key));
		}
		template <typename _T>
		std::shared_ptr<_T> make(const char *key) const
		{
			return std::make_shared<_T>(prototype<_T>(std::string(key)));
		}
	};
}

#endif