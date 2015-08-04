// name: renderer.cpp
// type: c++ class implementation
// desc: opengl rendering
// auth: is0urce

#include "stdafx.h"

#include "renderer.h"

#include "glsl.h"
#include "point.h"
#include "map.h"
#include "color.h"
#include "tile.h"

#include <stdexcept>

using namespace px;
using namespace px::shell;

namespace
{
	static const unsigned int range_width = 5;
	static const unsigned int range_height = range_width;
	static const unsigned int range_size = range_width * range_height;
	static const unsigned int vertice_depth = 2;
	static const unsigned int color_depth = 4;
	static const unsigned int points_quad = 4; // rasterise tiles in 4-points (2-trices)
	static const unsigned int index_quad = 6;

	const char* font_path_ui = "code2000.ttf";
	const char* font_path_notify = "code2000.ttf";
	const char* font_path_unit = "code2000.ttf";
	const unsigned int font_size_ui = 8;
	const unsigned int font_size_notify = 8;
	const unsigned int font_size_unit = 32;
}

renderer::renderer(renderer::opengl_handle opengl) : 
	m_ui_font(font_path_ui, font_size_ui),
	m_notify_font(font_path_notify, font_size_unit),
	m_unit_font(font_path_unit, font_size_unit)
{
	if (!opengl) throw std::runtime_error("renderer::renderer(renderer::opengl_handle opengl) opengl is null");

	m_opengl.swap(opengl);
	m_background.init({ vertice_depth, color_depth });
	m_program = glsl::program("shaders\\ground");
}

renderer::~renderer()
{
}

void renderer::fill_bg()
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
			color(1.0, 1.0, 0.0).write(&colors[color_offset + i * color_depth]);
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
	m_background.fill(range_size * points_quad, { &vertices[0], &colors[0] }, indices);
}

void renderer::draw(double span)
{
	m_opengl->update(m_width, m_height);
	if (m_width <= 0 || m_height <= 0) return;

	m_aspect = m_width;
	m_aspect /= m_height;

	glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);

	glUseProgram(m_program);
	glUniform1f(glGetUniformLocation(m_program, "aspect"), (GLfloat)m_aspect);
	glUniform1f(glGetUniformLocation(m_program, "scale"), 0.1f);

	fill_bg();

	m_background.draw();

	m_opengl->swap();
}