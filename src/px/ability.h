// name: ability.h
// type: c++ header
// desc: interface template class declaration
// auth: is0urce

// abstract ability used on target or on specified position

#ifndef PX_ABILITY_H
#define PX_ABILITY_H

#include <px/point.h>

namespace px
{
	template <typename _T>
	class ability
	{
	public:
		typename _T target_t;

	public:
		virtual ~ability() = 0 {}

	protected:
		virtual void use_ability(_T target) = 0;
		virtual void use_ability(const point &target) = 0;
		virtual bool useable_ability(_T target) const = 0;
		virtual bool useable_ability(const point &target) const = 0;
		virtual bool targeted_ability() const = 0;

	public:
		bool targeted() const { return targeted_ability(); }
		bool useable(_T target) { return useable_ability(target); } const
		bool useable(const point &target) { return useable_ability(target); } const
		void use(_T target) { use_ability(target); }
		void use(const point &target) { use_ability(target); }
	};
}

#endif