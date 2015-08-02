// name: renderer.cpp
// type: c++ class implementation
// desc: opengl rendering
// auth: is0urce

#include "stdafx.h"

#include "renderer.h"

#include "glsl.h"
#include "point.h"
#include "color.h"

#include <stdexcept>

using namespace px;
using namespace px::shell;

namespace
{
	static const unsigned int range_width = 3;
	static const unsigned int range_height = range_width;
	static const unsigned int range_size = range_width * range_height;
	static const unsigned int vertice_depth = 2;
	static const unsigned int color_depth = 4;
	static const unsigned int points_quad = 4; // rasterise tiles in 4-points (2-trices)
	static const unsigned int index_quad = 6;
}

renderer::renderer(renderer::opengl_handle opengl)
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
	point range(range_width, range_height);
	std::vector<GLfloat> vertices(range_size * points_quad * vertice_depth);
	std::vector<GLfloat> colors(range_size * points_quad * color_depth);
	std::vector<GLuint> indices(range_size * index_quad);

	// vertex attributes
	unsigned int vertex_offset = 0;
	unsigned int color_offset = 0;
	range.enumerate([&](const point &position)
	{
		position.moved(point(0, 0)).write(&vertices[vertex_offset + 0 * vertice_depth]);
		position.moved(point(0, 1)).write(&vertices[vertex_offset + 1 * vertice_depth]);
		position.moved(point(1, 1)).write(&vertices[vertex_offset + 2 * vertice_depth]);
		position.moved(point(1, 0)).write(&vertices[vertex_offset + 3 * vertice_depth]);
		for (unsigned int i = 0; i < points_quad; ++i)
		{
			//perception.GetAppearance(position).GetColor().Write(&color[coloroffset + i * colordepth]);
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
	m_aspect = m_width;
	m_aspect /= m_height;

	glUseProgram(m_program);
	//GLfloat vertice[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
	//GLfloat color[] = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f };
	//GLuint ax[] = { 0, 1, 2 };
	//m_background.fill(3, 3, { vertice, color }, ax);

	fill_bg();
	m_background.draw();

	m_opengl->swap();
}