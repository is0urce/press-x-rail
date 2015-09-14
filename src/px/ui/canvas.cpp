// name: Gui.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "canvas.h"

#include <px/string.h>

using namespace px;
using namespace px::ui;

canvas::canvas(point range) : map<symbol>(range), m_front(1, 1, 1, 1.0), m_back(0, 0, 0, 0.0)
{
}

canvas::~canvas()
{
}

void canvas::cls(const symbol &clear_color)
{
	fill(clear_color);
}

void canvas::cls()
{
	fill({ ' ', m_front, m_back });
}

void canvas::text_color(color front_color)
{
	m_front = front_color;
}

void canvas::draw_color(color back_color)
{
	m_back = back_color;
}

void canvas::rectangle(const point &start, const point &range)
{
	rectangle(start, range, m_back);
}

void canvas::rectangle(const point &start, const point &range, const color &back)
{
	range.enumerate(start, [&](const point &range_position)
	{
		if (in_range(range_position))
		{
			symbol &s = at(range_position);
			s.code = ' ';
			s.back = back;
		}
	});
}

void canvas::pset(const point& pos)
{
	pset(pos, m_back);
}

void canvas::pset(const point& position, const color& back)
{
	if (in_range(position))
	{
		at(position).back = back;
	}
}

void canvas::write(const point &position, symbol::code_t code, const color &front)
{
	if (in_range(position))
	{
		auto &s = at(position);
		s.code = code;
		s.front = front;
	}
}

void canvas::write(const point &position, symbol::code_t code)
{
	write(position, code, m_front);
}

void canvas::write(point position, const std::string &text, const color &front)
{
	string::enum_utf8(text, [&](symbol::code_t letter)
	{
		write(position, letter, front);
		++position.X;
	});
}

void canvas::write(point position, const std::string &text)
{
	write(position, text, m_front);
}

void canvas::write_integer(point position, int num, const color &front)
{
	bool negative = num < 0;
	if (negative)
	{
		num = -num;
	}

	do
	{
		write(position, '0' + num % 10, front);
		num /= 10;
		--position.X;
	} while (num > 0);

	if (negative)
	{
		write(position, '-', front);
	}
}

void canvas::write_integer(point position, int num)
{
	write_integer(position, num, m_front);
}