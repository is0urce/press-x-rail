// name: "font.cpp"
// type: c++
// desc: class implementation
// auth: is0urce

#include "stdafx.h"

#include "Font.h"

#include <algorithm>
#include <string>
#include <sstream>
#include <mutex>

#pragma comment(lib, "freetype.lib")

using namespace px::shell;

// CONSTANTS

namespace
{
	const unsigned int multiplier = 16; // atlas width in em
	const unsigned int base_fill = 128; // precashe size
	std::mutex font_mutex; // no mutexes for now as crt debug memory leak report issue
}

// IMPLEMENTATION

FT_Library font::m_lib;
unsigned int font::m_count = 0;


font::font(const char* path, unsigned int size)
	:
	m_size(size),
	m_penx(0), m_peny(0), m_next_line(0), 
	m_dirty(true)
{
	if (!path) throw std::runtime_error("Font::Font(const char* path, int size) path == 0");
	if (size == 0) throw std::runtime_error("Font::Font(const char* path, int size) size == 0");

	std::lock_guard<std::mutex> mutex_lock(font_mutex);

	// init freetype if first
	FT_Error error;
	if (m_count++ == 0)
	{
		error = FT_Init_FreeType(&m_lib);
		if (error)
		{
			throw std::runtime_error("FT_Init_FreeType");
		}
	}

	error = FT_New_Face(m_lib, path, 0, &m_face);
	if (error)
	{
		std::string message = std::string("FT_New_Face error path='") + path + "'";
		if (error == FT_Err_Unknown_File_Format)
		{
			throw std::runtime_error(message + " font format unknown");
		}
		else
		{
			throw std::runtime_error(message);
		}
	}
	error = FT_Set_Pixel_Sizes(m_face, 0, m_size);

	m_width = 1;
	while (m_width < m_size * multiplier)
	{
		m_width <<= 1;
	}
	m_height = m_width;
	m_atlas.reset(new char[m_width * m_height]);
	memset(m_atlas.get(), 0, sizeof(m_atlas[0]) * m_width * m_height);

	m_texture.width = m_width;
	m_texture.height = m_height;
	m_texture.data = m_atlas.get();

	// include base
	m_base.reset(new std::unique_ptr<glyph>[base_fill]);
	for (unsigned int i = 0; i < base_fill; ++i)
	{
		m_base[i] = raster(i);
	}
}

font::~font()
{
	std::lock_guard<std::mutex> mutex_lock(font_mutex);

	FT_Done_Face(m_face);
	if (--m_count == 0)
	{
		FT_Done_FreeType(m_lib);
	}
}

void font::include(unsigned int unicode_plus)
{
	include(unicode_plus, unicode_plus);
}

std::unique_ptr<glyph> font::raster(unsigned int unicode_plus)
{
	FT_Error error;
	int index = FT_Get_Char_Index(m_face, unicode_plus);
	error = FT_Load_Glyph(m_face, index, FT_LOAD_DEFAULT);
	if (error) throw std::runtime_error("FT_Load_Glyph");
	error = FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_NORMAL);
	if (error) throw std::runtime_error("FT_Render_Glyph");
	int w = m_face->glyph->bitmap.width;
	int h = m_face->glyph->bitmap.rows;

	int xstride = w + w / 4;
	int ystride = h + h / 4;
	if (m_penx + xstride > m_width)
	{
		m_penx = 0;
		m_peny += m_next_line;
		m_next_line = 0;
	}
	if (m_peny + ystride > m_height)
	{
		throw std::runtime_error("atlas too small");
	}

	std::unique_ptr<glyph> g(new glyph());
	g->advance = (m_face->glyph->advance.x >> 6) / double(m_size);
	g->top = double(m_peny) / m_height;
	g->bottom = double(m_peny + h) / m_height;
	g->left = double(m_penx) / m_width;
	g->right = double(m_penx + w) / m_width;
	g->pixwidth = w;
	g->pixheight = h;
	g->pixvertical = m_face->glyph->bitmap_left;
	g->pixhorisontal = m_face->glyph->bitmap_top;
	g->width = double(w) / m_size;
	g->height = double(h) / m_size;
	g->vertical = (m_face->glyph->bitmap_left) / double(m_size);
	g->horisontal = (m_face->glyph->bitmap_top) / double(m_size);

	// copy bitmap
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			m_atlas[(m_peny + j) * m_width + m_penx + i] = m_face->glyph->bitmap.buffer[i + j * w];
		}
	}
	m_penx += xstride;
	m_next_line = (std::max<unsigned int>)(m_next_line, ystride);

	m_dirty = true;
	return g;
}

void font::include(unsigned int uplus, unsigned int into)
{
	auto &g = m_letters[into];
	if (!g)
	{
		g = raster(uplus);
	}
}

const glyph& font::operator[](unsigned int uplus)
{
	if (uplus == 0)
	{
		throw std::runtime_error("Glyph& font::operator[](unsigned int uplus) uplus == 0");
	}
	if (uplus < base_fill) return *m_base[uplus];

	std::unique_ptr<glyph>& glyph = m_letters[uplus];
	if (!glyph)
	{
		glyph = raster(uplus);
	}
	return *glyph;
}


const font::font_texture& font::texture() const
{
	//m_dirty = false;
	return m_texture;
}

bool font::updated() const
{
	return m_dirty;
}

void font::updated(bool is_updated)
{
	m_dirty = is_updated;
}

unsigned int font::size() const
{
	return m_size;
}

double font::kerning(unsigned int l1, unsigned int l2) const
{
	FT_Vector kerning;
	FT_Get_Kerning(m_face, l1, l2, FT_KERNING_DEFAULT, &kerning);
	return (kerning.x >> 6) / double(m_size);
}