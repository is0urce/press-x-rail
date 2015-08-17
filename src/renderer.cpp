// name: renderer.cpp
// type: c++ class implementation
// desc: opengl rendering
// auth: is0urce

#include "stdafx.h"

#include "renderer.h"

#include "font.h"
#include "glsl.h"
#include "point.h"
#include "vector.h"
#include "color.h"
#include "perception.h"
#include "game.h"

#include <memory>
#include <stdexcept>
#include <vector>

using namespace px;
using namespace px::shell;

namespace
{
	const double camera_default = 0.1;
	const unsigned int range_width = game::perc_width;
	const unsigned int range_height = game::perc_height;
	const unsigned int range_size = range_width * range_height;
	const unsigned int vertice_depth = 2;
	const unsigned int color_depth = 4;
	const unsigned int texcoord_depth = 2;
	const unsigned int points_quad = 4; // rasterise tiles in 4-points (2-trices)
	const unsigned int index_quad = 6;

	const char* font_path_ui = "code2000.ttf";
	const char* font_path_notify = "code2000.ttf";
	const char* font_path_unit = "code2000.ttf";
	const unsigned int font_size_ui = 32;
	const unsigned int font_size_notify = 16;
	const unsigned int font_size_unit = 32;
}

renderer::renderer(renderer::opengl_handle opengl) : m_camera(camera_default)
{
	if (!opengl) throw std::runtime_error("renderer::renderer(renderer::opengl_handle opengl) opengl is null");

	m_opengl.swap(opengl);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	m_ui.font.reset(new font(font_path_ui, font_size_ui));
	m_notify.font.reset(new font(font_path_notify, font_size_unit));
	m_glyph.font.reset(new font(font_path_unit, font_size_unit));

	m_background.vao.init({ vertice_depth, color_depth });
	m_background.program = glsl::program("shaders\\ground");

	m_tiles.vao.init({ vertice_depth, texcoord_depth, color_depth });
	m_tiles.program = glsl::program("shaders\\units");

	m_units.vao.init({ vertice_depth, texcoord_depth, color_depth });
	m_units.program = glsl::program("shaders\\units");

	glGenTextures(1, &m_ui.texture);
	glGenTextures(1, &m_notify.texture);
	glGenTextures(1, &m_glyph.texture);

	const font::font_texture &ui_texture = m_ui.font->texture();
	glBindTexture(GL_TEXTURE_2D, m_ui.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, ui_texture.width, ui_texture.height, 0, GL_RED, GL_UNSIGNED_BYTE, ui_texture.data);
	glEnable(GL_TEXTURE_2D);
	glGenerateMipmap(GL_TEXTURE_2D);  // generate mipmaps here.
	// mipmap filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); // filtering, sharp switching between mipmaps
}

renderer::~renderer()
{
	m_background.vao.clear();
	m_tiles.vao.clear();
	m_units.vao.clear();

	glDeleteProgram(m_background.program);
	glDeleteProgram(m_tiles.program);
	glDeleteProgram(m_units.program);

	glDeleteTextures(1, &m_ui.texture);
	glDeleteTextures(1, &m_notify.texture);
	glDeleteTextures(1, &m_glyph.texture);
}

void renderer::fill_bg(const perception_t& perception)
{
	std::vector<GLfloat> vertices(range_size * points_quad * vertice_depth);
	std::vector<GLfloat> colors(range_size * points_quad * color_depth);
	std::vector<GLuint> indices(range_size * index_quad);

	// vertex attributes
	unsigned int vertex_offset = 0;
	unsigned int color_offset = 0;
	point(range_width, range_height).enumerate([&](const point &position)
	{
		position.moved(point(0, 0)).write(&vertices[vertex_offset + 0 * vertice_depth]);
		position.moved(point(0, 1)).write(&vertices[vertex_offset + 1 * vertice_depth]);
		position.moved(point(1, 1)).write(&vertices[vertex_offset + 2 * vertice_depth]);
		position.moved(point(1, 0)).write(&vertices[vertex_offset + 3 * vertice_depth]);
		for (unsigned int i = 0; i < points_quad; ++i)
		{
			perception.ground(position).write(&colors[color_offset + i * color_depth]);
		}

		vertex_offset += vertice_depth * points_quad;
		color_offset += color_depth * points_quad;
	});

	// indices
	unsigned int indexoffset = 0;
	for (unsigned int i = 0; i < range_size; ++i)
	{
		indices[indexoffset + 0] = indices[indexoffset + 3] = i * points_quad + 0;
		indices[indexoffset + 1] = indices[indexoffset + 5] = i * points_quad + 2;
		indices[indexoffset + 2] = i * points_quad + 1;
		indices[indexoffset + 4] = i * points_quad + 3;
		indexoffset += index_quad;
	}

	// bind
	m_background.vao.fill(range_size * points_quad, { &vertices[0], &colors[0] }, indices);
}

void renderer::fill_tiles(const perception_t& perception)
{
	std::vector<GLfloat> vertices(range_size * points_quad * vertice_depth);
	std::vector<GLfloat> texture(range_size * points_quad * vertice_depth);
	std::vector<GLfloat> colors(range_size * points_quad * color_depth);
	std::vector<GLuint> indices(range_size * index_quad);

	// vertex attributes
	font &font = *m_ui.font;
	unsigned int vertex_offset = 0;
	unsigned int color_offset = 0;
	unsigned int texture_offset = 0;
	point(range_width, range_height).enumerate([&](const point &position)
	{
		auto g = font[perception.appearance(position)];

		double width = g.width / 2;
		double height = g.height / 2;
		(position + vector(0.5f - width, 0.5f - height)).write(&vertices[vertex_offset + 0 * vertice_depth]);
		(position + vector(0.5f - width, 0.5f + height)).write(&vertices[vertex_offset + 1 * vertice_depth]);
		(position + vector(0.5f + width, 0.5f + height)).write(&vertices[vertex_offset + 2 * vertice_depth]);
		(position + vector(0.5f + width, 0.5f - height)).write(&vertices[vertex_offset + 3 * vertice_depth]);

		texture[texture_offset + 0] = texture[texture_offset + 2] = (GLfloat)g.left;
		texture[texture_offset + 1] = texture[texture_offset + 7] = (GLfloat)g.bottom;
		texture[texture_offset + 4] = texture[texture_offset + 6] = (GLfloat)g.right;
		texture[texture_offset + 3] = texture[texture_offset + 5] = (GLfloat)g.top;

		for (unsigned int i = 0; i < points_quad; ++i)
		{
			(perception.hide(position) ? color(0) : color(1.0, 1.0, 1.0)).write(&colors[color_offset + i * color_depth]);
		}

		vertex_offset += vertice_depth * points_quad;
		color_offset += color_depth * points_quad;
		texture_offset += texcoord_depth * points_quad;
	});

	// indices
	unsigned int indexoffset = 0;
	for (unsigned int i = 0; i < range_size; ++i)
	{
		indices[indexoffset + 0] = indices[indexoffset + 3] = i * points_quad + 0;
		indices[indexoffset + 1] = indices[indexoffset + 5] = i * points_quad + 2;
		indices[indexoffset + 2] = i * points_quad + 1;
		indices[indexoffset + 4] = i * points_quad + 3;
		indexoffset += index_quad;
	}

	// bind
	m_tiles.vao.fill(range_size * points_quad, { &vertices[0], &texture[0], &colors[0] }, indices);
}

void renderer::fill_units(const perception_t& perception)
{
	unsigned int unit_num = perception.unit_count();

	std::vector<GLfloat> vertices(unit_num * points_quad * vertice_depth);
	std::vector<GLfloat> texture(unit_num * points_quad * vertice_depth);
	std::vector<GLfloat> colors(unit_num * points_quad * color_depth);
	std::vector<GLuint> indices(unit_num * index_quad);

	// vertex attributes
	font &font = *m_ui.font;
	unsigned int vertex_offset = 0;
	unsigned int color_offset = 0;
	unsigned int texture_offset = 0;
	perception.enumerate_units([&](const perception::avatar_t& unit)
	{
		auto g = font[unit.appearance()];
		point position = unit.position();

		double width = g.width / 2;
		double height = g.height / 2;
		(position + vector(0.5f - width, 0.5f - height)).write(&vertices[vertex_offset + 0 * vertice_depth]);
		(position + vector(0.5f - width, 0.5f + height)).write(&vertices[vertex_offset + 1 * vertice_depth]);
		(position + vector(0.5f + width, 0.5f + height)).write(&vertices[vertex_offset + 2 * vertice_depth]);
		(position + vector(0.5f + width, 0.5f - height)).write(&vertices[vertex_offset + 3 * vertice_depth]);

		texture[texture_offset + 0] = texture[texture_offset + 2] = (GLfloat)g.left;
		texture[texture_offset + 1] = texture[texture_offset + 7] = (GLfloat)g.bottom;
		texture[texture_offset + 4] = texture[texture_offset + 6] = (GLfloat)g.right;
		texture[texture_offset + 3] = texture[texture_offset + 5] = (GLfloat)g.top;

		for (unsigned int i = 0; i < points_quad; ++i)
		{
			color(1.0, 1.0, 1.0).write(&colors[color_offset + i * color_depth]);
		}

		vertex_offset += vertice_depth * points_quad;
		color_offset += color_depth * points_quad;
		texture_offset += texcoord_depth * points_quad;
	});

	// indices
	unsigned int indexoffset = 0;
	for (unsigned int i = 0; i < unit_num; ++i)
	{
		indices[indexoffset + 0] = indices[indexoffset + 3] = i * points_quad + 0;
		indices[indexoffset + 1] = indices[indexoffset + 5] = i * points_quad + 2;
		indices[indexoffset + 2] = i * points_quad + 1;
		indices[indexoffset + 4] = i * points_quad + 3;
		indexoffset += index_quad;
	}

	// bind
	m_units.vao.fill(unit_num * points_quad, { &vertices[0], &texture[0], &colors[0] }, indices);
}

void renderer::draw(const perception_t &perception, double span)
{
	if (perception.width() < range_width || perception.height() < range_height) throw std::logic_error("renderer::draw(..) - perception < range");

	m_opengl->update(m_width, m_height);
	if (m_width <= 0 || m_height <= 0) return;

	fill_bg(perception);
	fill_tiles(perception);
	fill_units(perception);

	point center = perception.range() / 2;
	GLfloat aspect = (GLfloat)(m_width) / m_height;
	GLfloat scale = (GLfloat)m_camera;
	GLfloat x_center = (GLfloat)center.X;
	GLfloat y_center = (GLfloat)center.Y;

	glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_background.program);
	glUniform1f(glGetUniformLocation(m_background.program, "aspect"), aspect);
	glUniform1f(glGetUniformLocation(m_background.program, "scale"), scale);
	glUniform2f(glGetUniformLocation(m_background.program, "center"), x_center, y_center);
	glBindTexture(GL_TEXTURE_2D, m_ui.texture);
	m_background.vao.draw();

	glUseProgram(m_tiles.program);
	glUniform1f(glGetUniformLocation(m_tiles.program, "aspect"), aspect);
	glUniform1f(glGetUniformLocation(m_tiles.program, "scale"), scale);
	glUniform2f(glGetUniformLocation(m_tiles.program, "center"), x_center, y_center);
	glUniform1i(glGetUniformLocation(m_tiles.program, "img"), 0);
	glBindTexture(GL_TEXTURE_2D, m_ui.texture);
	m_tiles.vao.draw();

	glUseProgram(m_units.program);
	glUniform1f(glGetUniformLocation(m_units.program, "aspect"), aspect);
	glUniform1f(glGetUniformLocation(m_units.program, "scale"), scale);
	glUniform2f(glGetUniformLocation(m_units.program, "center"), x_center, y_center);
	glUniform1i(glGetUniformLocation(m_units.program, "img"), 0);
	glBindTexture(GL_TEXTURE_2D, m_ui.texture);
	m_units.vao.draw();

	m_opengl->swap();
}

void renderer::scale(double pan)
{
	m_camera *= 1.0f + (pan / 1000.0f);
	m_camera = (std::min)(m_camera, 10.0);
	m_camera = (std::max)(m_camera, 0.01);
}