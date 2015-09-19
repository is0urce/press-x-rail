// name: color.h
// type: c++ header
// desc: class with implementation
// auth: is0urce

// color with real rgba components

#ifndef PX_COLOR_H
#define PX_COLOR_H

#include <cmath>

namespace px
{
	// hide constants
	namespace
	{
		const static double pi = 3.141592653589793238463; // more than enought
		const static unsigned int depth = 4;
	}
	class color
	{
	public:
		typedef double component;

		// attributes
	public:
		component R;
		component G;
		component B;
		component A;

		// ctor & dtor
	public:
		color() : R{}, G{}, B{}, A{} {};
		color(component r, component g, component b) : R(r), G(g), B(b), A(1) {};
		color(component r, component g, component b, component a) : R(r), G(g), B(b), A(a) {};
		color(unsigned int hex) { set_hex(hex); }

	public:
		static color rgb(int r, int g, int b) { return color(r / 255.0, g / 255.0, b / 255.0); }
		static color rgba(int r, int g, int b, int a) { return color(r / 255.0, g / 255.0, b / 255.0, a / 255.0); }
		static color black() { return color(0, 0, 0, 1); };
		static color white() { return color(1, 1, 1, 1); };
		static color transparent() { return color(0, 0, 0, 0); };

		// operators

		color operator-() const { return color(-R, -G, -B); };
		color& operator+=(const color &color) { R += color.R; G += color.G; B += color.B; A += color.A; return *this; };
		color& operator-=(const color &color) { R -= color.R; G -= color.G; B -= color.B; A -= color.A; return *this; };
		color& operator*=(const color &color) { R *= color.R, G *= color.G, B *= color.B, A *= color.A; return *this; };

		bool operator==(const color &color) const { return R == color.R && G == color.G && B == color.B && A == color.A; }
		bool operator!=(const color &color) const { return !(*this == color); }
		color operator+(const color &color) const { auto result = *this; return result += color; }
		color operator-(const color &color) const { auto result = *this; return result -= color; }
		color operator*(const color &color) const { auto result = *this; return result *= color; }

		color operator*(component c) const { return color(R * c, G * c, B * c, A * c); };
		color operator/(component c) const { return color(R / c, G / c, B / c, A / c); };
		color operator*=(component c) { *this = *this * c; return *this; };
		color operator/=(component c) { *this = *this / c; return *this; };

		template <typename _M>
		void write(_M *memory) const { memory[0] = (_M)R; memory[1] = (_M)G; memory[2] = (_M)B; memory[3] = (_M)A; };
		template <typename _M>
		void write(_M *memory, unsigned int repeat) const { for (unsigned int i = 0; i < repeat; ++i) { write(memory); memory += depth; } };
		void write(component *memory) const { memory[0] = R; memory[1] = G; memory[2] = B; memory[3] = A; };
		void write(component *memory, unsigned int repeat) const { for (unsigned int i = 0; i < repeat; ++i) { write(memory); memory += depth; } };
		void set_rgba(int r, int g, int b, int a) { R = r / 255.0; G = g / 255.0; B = b / 255.0; A = a / 255.0; }
		void set_rgb(int r, int g, int b) { R = r / 255.0; G = g / 255.0; B = b / 255.0; A = 1.0; }
		void set_hex(unsigned int hex) { set_rgb(hex / 256 / 256 % 256, hex / 256 % 256, hex % 256); };
		void shift_hue(double angle) { *this = transform_hue(*this, angle); };
		void shift_brightness(double scale) { *this = transform_hsv(*this, 0, 1.0, scale); }
		void shift_hsv(double hue, double saturation, double v) { *this = transform_hsv(*this, hue, saturation, v); };
		color transform_hsv(double hue, double saturation, double v) const { return transform_hsv(*this, hue, saturation, v); };
		color average(const color &other) const { return (*this + other) / 2; };
		double luminance() const { return 0.2125 * R + 0.7154 * G + 0.0721 * B; };
		color lerp(const color b, component t) const { return *this * (t - 1) + b * t; }

		// hue - hue shift (in degrees) saturation - saturation multiplier (scalar), v - value multiplier (scalar)
		static color transform_hsv(const color &in, double hue, double saturation, double V)
		{
			double VSU = V * saturation * std::cos(hue * pi / 180.0);
			double VSW = V * saturation * std::sin(hue * pi / 180.0);

			color ret(in);
			ret.R = (.299*V + .701*VSU + .168*VSW)*in.R
				+ (.587*V - .587*VSU + .330*VSW)*in.G
				+ (.114*V - .114*VSU - .497*VSW)*in.B;
			ret.G = (.299*V - .299*VSU - .328*VSW)*in.R
				+ (.587*V + .413*VSU + .035*VSW)*in.G
				+ (.114*V - .114*VSU + .292*VSW)*in.B;
			ret.B = (.299*V - .3*VSU + 1.25*VSW)*in.R
				+ (.587*V - .588*VSU - 1.05*VSW)*in.G
				+ (.114*V + .886*VSU - .203*VSW)*in.B;

			return ret;
		}
		static color transform_hue(const color &in, double angle)
		{
			double U = std::cos(angle * pi / 180.0);
			double W = std::sin(angle * pi / 180.0);

			color ret(in);
			ret.R =	(.299 + .701*U + .168*W) * in.R
				+ (.587 - .587*U + .330*W) * in.G
				+ (.114 - .114*U - .497*W) * in.B;
			ret.G = (.299 - .299*U - .328*W) * in.R
				+ (.587 + .413*U + .035*W) * in.G
				+ (.114 - .114*U + .292*W) * in.B;
			ret.B = (.299 - .3*U + 1.25*W) * in.R
				+ (.587 - .588*U - 1.05*W) * in.G
				+ (.114 + .886*U - .203*W) * in.B;

			return ret;
		}
		static color average(const color &a, const color &b) { return (a + b) / 2; };
		static color average(const color &a, const color &b, const color &c) { return (a + b + c) / 3; };
		static color average(const color &a, const color &b, const color &c, const color &d) { return (a + b + c + d) / 4; };
	};
}

#endif