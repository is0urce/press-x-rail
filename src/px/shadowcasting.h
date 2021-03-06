// name: shadowcasting.h
// type: c++ header & implementation
// auth: is0urce
// date: 2014-12-16

#ifndef PX_SHADOWCASTING_H
#define PX_SHADOWCASTING_H

#include <px/point.h>

#include <memory>
#include <functional>
#include <stdexcept>

namespace px
{
	// octants
	static int multipliers[4][8] =
	{
		{ 1, 0, 0, -1, -1, 0, 0, 1 },
		{ 0, 1, -1, 0, 0, -1, 1, 0 },
		{ 0, 1, 1, 0, 0, -1, -1, 0 },
		{ 1, 0, 0, 1, -1, 0, 0, -1 }
	};

	class shadowcasting
	{
	public:
		typedef std::function<bool(const point &position)> map_fn; // std::function<bool(const point &position)>

	private:
		std::unique_ptr<bool[]> m_fov;
		unsigned int m_radius;
		unsigned int m_width;
		unsigned int m_len;
		point m_offset;

	public:
		shadowcasting(unsigned int radius_size)
		{
			radius(radius_size);
			clear();
		}
		shadowcasting(std::function<bool(const point &position)> map_fn, const point &start, unsigned int radius_size)
		{
			radius(radius_size);
			recursive(map_fn, start, radius_size);
		}
		~shadowcasting()
		{
		}

		void radius(unsigned int radius_size)
		{
			if (radius_size == 0) throw std::logic_error("px::shadowcasting::radius(...) radius_size == 0");
			m_radius = radius_size;
			m_width = m_radius * 2 + 1;
			m_len = m_width * m_width;
			m_fov = std::unique_ptr<bool[]>(new bool[m_len]);
		}
		void clear()
		{
			for (unsigned int i = 0; i < m_len; ++i)
			{
				m_fov[i] = false;
			}
		}
		void light(point position)
		{
			position += point(m_radius, m_radius) - m_offset;

			unsigned int pos = position.Y * m_width + position.X;
			if (pos == 0 || pos >= m_len)
			{
				throw std::runtime_error("px::shadowcasting::light(..) point out of range");
			}

			m_fov[pos] = true;
		}
		bool in_fov(point position) const
		{
			position += point(m_radius, m_radius) - m_offset;

			if (position.X < 0 || position.Y < 0 || position.X >= (int)m_width || position.Y >= (int)m_width) return false;

			return m_fov[position.Y * m_width + position.X];
		}

		void recursive(std::function<bool(const point &position)> map_fn, const point &start)
		{
			recursive(map_fn, start, m_radius);
		}
		void recursive(std::function<bool(const point &position)> map_fn, const point &start, unsigned int radius_size)
		{
			if (radius_size == 0) throw std::runtime_error("px::shadowcasting::recursive(..) radius_size == 0");
			if (radius_size > m_radius)
			{
				radius(radius_size);
			}

			clear();
			m_offset = start;
			light(start);
			for (unsigned int i = 0; i < 8; ++i) // calculate every octant
			{
				recursive(map_fn, start.X, start.Y, radius_size, 1, 1.0, 0.0, multipliers[0][i], multipliers[1][i], multipliers[2][i], multipliers[3][i]);
			}
		}

	private:
		void recursive(std::function<bool(const point &position)> map_fn, int x, int y, unsigned int radius_size, unsigned int row, float start, float end, int xx, int xy, int yx, int yy)
		{
			if (start < end) return;
			unsigned int radius2 = radius_size * radius_size;

			float next_start_slope = start;
			for (unsigned int i = row; i <= radius_size; ++i)
			{
				int dx = 0 - i - 1;
				int dy = 0 - i;
				bool blocked = false;
				while (dx <= 0)
				{
					++dx;
					float l_slope = (dx - 0.5f) / (dy + 0.5f);
					float r_slope = (dx + 0.5f) / (dy - 0.5f);

					if (start < r_slope)
					{
						continue;
					}
					else if (end > l_slope)
					{
						break;
					}
					else
					{
						point absolute(x + dx * xx + dy * xy, y + dx * yx + dy * yy);

						// to avoid behind-collumn peek you have to see center of a tile to actualy see it (or it shoult be wall)
						float slope = (float)(dx) / (float)(dy);
						bool wall = !map_fn(absolute);
						bool center = start >= slope && slope >= end;
						if ((center || wall) && (dx * dx + dy * dy < (int)radius2))
						{
							light(absolute);
						}

						if (blocked)
						{
							if (wall)
							{
								next_start_slope = r_slope;
								continue;
							}
							else
							{
								blocked = false;
								start = next_start_slope;
							}
						}
						else
						{
							if (wall && i < radius_size)
							{
								blocked = true;
								recursive(map_fn, x, y, radius_size, i + 1, start, l_slope, xx, xy, yx, yy);
								next_start_slope = r_slope;
							}
						}
					}
				}
				if (blocked) break;
			}
		}
	};
}

#endif