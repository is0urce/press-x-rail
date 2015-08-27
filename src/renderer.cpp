// name: renderer.cpp
// type: c++ class implementation
// desc: opengl rendering
// auth: is0urce

#include "stdafx.h"

#include "renderer.h"

#include "font.h"
#include "glsl.h"
#include "vector.h"
#include "color.h"
#include "perception.h"
#include "game.h"
#include "string.h"

#include <memory>
#include <stdexcept>
#include <vector>

using namespace px;
using namespace px::shell;

namespace
{
	static const double camera_default = 0.1;
	static const unsigned int range_width = game::perc_width;
	static const unsigned int range_height = game::perc_height;
	static const point range(range_width, range_height);
	static const unsigned int range_size = range_width * range_height;
	static const unsigned int vertice_depth = 2;
	static const unsigned int color_depth = 4;
	static const unsigned int texcoord_depth = 2;
	static const unsigned int points_quad = 4; // rasterise tiles in 4-points (2-trices)
	static const unsigned int index_quad = 6;

	static const char *font_path_ui = "code2000.ttf";
	static const char *font_path_notify = "FiraMono-Bold.ttf";
	static const char *font_path_unit = "code2000.ttf";
	static const unsigned int font_size_ui = 64;
	static const unsigned int font_size_notify = 32;
	static const unsigned int font_size_unit = 64;

	inline void fill_vertex(const vector &position, const vector &range, GLfloat *dest)
	{
		(position + vector(-0.5, -0.5) * range).write(dest + 0 * vertice_depth);
		(position + vector(-0.5, 0.5) * range).write(dest + 1 * vertice_depth);
		(position + vector(0.5, 0.5) * range).write(dest + 2 * vertice_depth);
		(position + vector(0.5, -0.5) * range).write(dest + 3 * vertice_depth);
	}
	inline void fill_vertex(const vector &position, GLfloat range, GLfloat *dest)
	{
		fill_vertex(position, { range, range }, dest);
	}
	inline void fill_vertex(const vector &position, GLfloat *dest)
	{
		fill_vertex(position, { 1, 1 }, dest);
	}
	inline void fill_texture(GLfloat left, GLfloat bottom, GLfloat right, GLfloat top, GLfloat *dest)
	{
		dest[0] = dest[2] = left;
		dest[1] = dest[7] = bottom;
		dest[4] = dest[6] = right;
		dest[3] = dest[5] = top;
	}
	inline void fill_index(unsigned int num, GLuint *dest)
	{
		unsigned int index_offset = 0;
		for (unsigned int i = 0; i < num; ++i)
		{
			dest[index_offset + 0] = dest[index_offset + 3] = i * points_quad + 0;
			dest[index_offset + 1] = dest[index_offset + 5] = i * points_quad + 2;
			dest[index_offset + 2] = i * points_quad + 1;
			dest[index_offset + 4] = i * points_quad + 3;
			index_offset += index_quad;
		}
	}
	inline void fill_color(const color &c, GLfloat *dest)
	{
		c.write(dest, points_quad);
	}
	inline void fill_index(unsigned int num, std::vector<GLuint> &dest)
	{
		fill_index(num, &dest[0]);
	}
}

renderer::renderer(renderer::opengl_handle opengl) : m_aspect(1), m_scale(camera_default), m_scene_size(0)
{
	if (!opengl) throw std::runtime_error("renderer::renderer(renderer::opengl_handle opengl) opengl is null");

	m_opengl.swap(opengl);

	m_ui.font.reset(new font(font_path_ui, font_size_ui));
	m_popup.font.reset(new font(font_path_notify, font_size_notify));
	m_glyph.font.reset(new font(font_path_unit, font_size_unit));

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glClampColor(GL_CLAMP_VERTEX_COLOR, GL_FALSE);
	glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
	glClampColor(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);

	m_background.vao.init({ vertice_depth, color_depth });
	m_background.program = glsl::program("shaders\\ground");

	m_tiles.vao.init({ vertice_depth, texcoord_depth, color_depth });
	m_tiles.program = glsl::program("shaders\\units");

	m_units.vao.init({ vertice_depth, texcoord_depth, color_depth });
	m_units.program = glsl::program("shaders\\units");

	m_scene.vao.init({ vertice_depth, texcoord_depth });
	m_scene.program = glsl::program("shaders\\scene");

	m_light.vao.init({ vertice_depth });
	m_light.program = glsl::program("shaders\\light");

	m_notification.vao.init({ vertice_depth, texcoord_depth, color_depth });
	m_notification.program = glsl::program("shaders\\notify");

	glGenFramebuffers(1, &m_scene_frame);
	glGenFramebuffers(1, &m_light_frame);

	glGenSamplers(1, &m_sampler);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glSamplerParameteri(m_sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 64);

	glGenTextures(1, &m_ui.texture);
	glGenTextures(1, &m_popup.texture);
	glGenTextures(1, &m_glyph.texture);
	glGenTextures(1, &m_scene_texture);
	glGenTextures(1, &m_light_texture);

	const font::font_texture &ui_texture = m_ui.font->texture();
	glBindTexture(GL_TEXTURE_2D, m_ui.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, ui_texture.width, ui_texture.height, 0, GL_RED, GL_UNSIGNED_BYTE, ui_texture.data);
	glEnable(GL_TEXTURE_2D);
	glGenerateMipmap(GL_TEXTURE_2D);

	const font::font_texture &popup_texture = m_popup.font->texture();
	glBindTexture(GL_TEXTURE_2D, m_popup.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, popup_texture.width, popup_texture.height, 0, GL_RED, GL_UNSIGNED_BYTE, popup_texture.data);
	glEnable(GL_TEXTURE_2D);
	glGenerateMipmap(GL_TEXTURE_2D);

	const font::font_texture &glyph_texture = m_glyph.font->texture();
	glBindTexture(GL_TEXTURE_2D, m_glyph.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, glyph_texture.width, glyph_texture.height, 0, GL_RED, GL_UNSIGNED_BYTE, glyph_texture.data);
	glEnable(GL_TEXTURE_2D);
	glGenerateMipmap(GL_TEXTURE_2D);
}

renderer::~renderer()
{
	m_background.vao.clear();
	m_tiles.vao.clear();
	m_units.vao.clear();
	m_scene.vao.clear();
	m_light.vao.clear();

	glDeleteProgram(m_background.program);
	glDeleteProgram(m_tiles.program);
	glDeleteProgram(m_units.program);

	glDeleteFramebuffers(1, &m_scene_frame);
	glDeleteFramebuffers(1, &m_light_frame);

	glDeleteTextures(1, &m_ui.texture);
	glDeleteTextures(1, &m_popup.texture);
	glDeleteTextures(1, &m_glyph.texture);
	glDeleteTextures(1, &m_scene_texture);
	glDeleteTextures(1, &m_light_texture);

	glDeleteSamplers(1, &m_sampler);
}

void renderer::setup_scene()
{
	auto max = (std::max)(m_width, m_height);
	unsigned int size = max > 0 ? max : 1;

	if (m_scene_size < size)
	{
		if (m_scene_size == 0) m_scene_size = 1;
		while (m_scene_size < size) m_scene_size <<= 1;

		// textures
		glBindTexture(GL_TEXTURE_2D, m_scene_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_scene_size, m_scene_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glBindTexture(GL_TEXTURE_2D, m_light_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_scene_size, m_scene_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		// framebuffers
		glBindFramebuffer(GL_FRAMEBUFFER, m_scene_frame);
		glBindTexture(GL_TEXTURE_2D, m_scene_texture);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_scene_texture, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("scene framebuffer not complete");

		glBindFramebuffer(GL_FRAMEBUFFER, m_light_frame);
		glBindTexture(GL_TEXTURE_2D, m_light_texture);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_light_texture, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("light framebuffer not complete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// merge vao
		std::vector<GLfloat> vertices(points_quad * vertice_depth);
		std::vector<GLfloat> texture(points_quad * texcoord_depth);
		std::vector<GLuint> indices(index_quad);

		fill_vertex({ 0, 0 }, 2, &vertices[0]);
		fill_texture(0, 0, 1, 1, &texture[0]);
		fill_index(1, &indices[0]);

		m_scene.vao.fill(points_quad, { &vertices[0], &texture[0] }, indices);
	}
}

void renderer::fill_bg(const perception_t &perception)
{
	std::vector<GLfloat> vertices(range_size * points_quad * vertice_depth);
	std::vector<GLfloat> colors(range_size * points_quad * color_depth);
	std::vector<GLuint> indices(range_size * index_quad);

	// vertex attributes
	unsigned int vertex_offset = 0;
	unsigned int color_offset = 0;
	range.enumerate([&](const point &position)
	{
		fill_vertex(position, &vertices[vertex_offset]);
		fill_color(perception.ground(position), &colors[color_offset]);

		vertex_offset += vertice_depth * points_quad;
		color_offset += color_depth * points_quad;
	});

	fill_index(range_size, indices);

	// bind
	m_background.vao.fill(range_size * points_quad, { &vertices[0], &colors[0] }, indices);
}

void renderer::fill_tiles(const perception_t &perception, font &fnt)
{
	std::vector<GLfloat> vertices(range_size * points_quad * vertice_depth);
	std::vector<GLfloat> textures(range_size * points_quad * vertice_depth);
	std::vector<GLfloat> colors(range_size * points_quad * color_depth);
	std::vector<GLuint> indices(range_size * index_quad);

	// vertex attributes
	//font &font = *m_ui.font;
	unsigned int vertex_offset = 0;
	unsigned int color_offset = 0;
	unsigned int texture_offset = 0;
	point(range_width, range_height).enumerate([&](const point &position)
	{
		auto g = fnt[perception.appearance(position)];

		fill_vertex(position, { g.width, g.height }, &vertices[vertex_offset]);
		fill_texture((GLfloat)g.left, (GLfloat)g.bottom, (GLfloat)g.right, (GLfloat)g.top, &textures[texture_offset]);
		fill_color(perception.hide(position) ? color(0, 0, 0, 0.0) : color(1, 1, 1, 1.0), &colors[color_offset]);

		vertex_offset += vertice_depth * points_quad;
		color_offset += color_depth * points_quad;
		texture_offset += texcoord_depth * points_quad;
	});

	// indices
	fill_index(range_size, indices);

	// bind
	m_tiles.vao.fill(range_size * points_quad, { &vertices[0], &textures[0], &colors[0] }, indices);
}

void renderer::fill_units(const perception_t &perception, font &fnt)
{
	unsigned int unit_num = perception.unit_count();

	std::vector<GLfloat> vertices(unit_num * points_quad * vertice_depth);
	std::vector<GLfloat> textures(unit_num * points_quad * vertice_depth);
	std::vector<GLfloat> colors(unit_num * points_quad * color_depth);
	std::vector<GLuint> indices(unit_num * index_quad);

	// vertex attributes
	unsigned int vertex_offset = 0;
	unsigned int color_offset = 0;
	unsigned int texture_offset = 0;
	perception.enumerate_units([&](const perception::avatar_t &unit)
	{
		auto g = fnt[unit.appearance()];

		fill_vertex(unit.position(), { g.width, g.height }, &vertices[vertex_offset]);
		fill_texture((GLfloat)g.left, (GLfloat)g.bottom, (GLfloat)g.right, (GLfloat)g.top, &textures[texture_offset]);
		fill_color(0xffffff, &colors[color_offset]);

		vertex_offset += vertice_depth * points_quad;
		color_offset += color_depth * points_quad;
		texture_offset += texcoord_depth * points_quad;
	});

	fill_index(unit_num, indices);

	// bind
	m_units.vao.fill(unit_num * points_quad, { &vertices[0], &textures[0], &colors[0] }, indices);
}

void renderer::fill_notifications(const perception_t &perception, font &fnt)
{
	// calculate number of required letter-squares
	unsigned int letters_num = 0;
	perception.enumerate_notifications([&](const notification &n)
	{
		letters_num += n.text.length();
	});

	std::vector<GLfloat> vertices(letters_num * points_quad * vertice_depth);
	std::vector<GLfloat> textures(letters_num * points_quad * vertice_depth);
	std::vector<GLfloat> colors(letters_num * points_quad * color_depth);
	std::vector<GLuint> indices(letters_num * index_quad);

	// vertex attributes
	unsigned int vertex_offset = 0;
	unsigned int color_offset = 0;
	unsigned int texture_offset = 0;
	perception.enumerate_notifications([&](const notification &n)
	{
		vector pen = n.position;
		auto &str = n.text;
		auto len = str.length();

		unsigned int prev_code = 0;
		double total_width = 0;
		string::enum_utf8(n.text, [&](unsigned int code)
		{
			auto glyph = fnt[code];
			total_width += fnt.kerning(prev_code, code);
			total_width += glyph.advance;
			prev_code = code;
		});

		pen.move(total_width * -n.size / 2, 0);

		prev_code = 0;
		string::enum_utf8(n.text, [&](unsigned int code)
		{
			auto g = fnt[code];
			pen.move(fnt.kerning(prev_code, code) * n.size, 0);
			prev_code = code;

			double gw = g.width * n.size;
			double gh = g.height * n.size;
			double ghor = g.horisontal * n.size;

			fill_vertex(pen.moved({ gw / 2, ghor - gh / 2 }), { gw, gh }, &vertices[vertex_offset]);
			fill_texture((GLfloat)g.left, (GLfloat)g.bottom, (GLfloat)g.right, (GLfloat)g.top, &textures[texture_offset]);
			fill_color(n.colour, &colors[color_offset]);

			pen.X += g.advance * n.size;

			vertex_offset += vertice_depth * points_quad;
			color_offset += color_depth * points_quad;
			texture_offset += texcoord_depth * points_quad;
		});
	});

	fill_index(letters_num, indices);

	// bind
	m_notification.vao.fill(letters_num * points_quad, { &vertices[0], &textures[0], &colors[0] }, indices);
}

void renderer::draw(const perception_t &perception, timespan_t timespan)
{
	if (perception.range() != range) throw std::logic_error("renderer::draw(..) - perception != range");

	m_opengl->update(m_width, m_height);
	if (m_width <= 0 || m_height <= 0) return;
	m_aspect = m_width;
	m_aspect /= m_height;
	timespan_t delta = (std::max)(timespan - m_last, 0.0);
	m_last = timespan;

	setup_scene(); // framebuffers & their textures

	fill_bg(perception);
	fill_tiles(perception, *m_ui.font);
	fill_units(perception, *m_ui.font);
	fill_notifications(perception, *m_popup.font);

	double movement_phase = std::min(timespan * 5.0, 1.0);
	vector center = perception.range() / 2 - (vector)perception.movement() * (1.0 - movement_phase);
	GLfloat aspect = (GLfloat)m_aspect;
	GLfloat scale = (GLfloat)m_scale;
	GLfloat x_center = (GLfloat)center.X;
	GLfloat y_center = (GLfloat)center.Y;

	// geometry
	glBindFramebuffer(GL_FRAMEBUFFER, m_scene_frame);
	glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(m_background.program);
	glUniform1f(glGetUniformLocation(m_background.program, "aspect"), aspect);
	glUniform1f(glGetUniformLocation(m_background.program, "scale"), scale);
	glUniform2f(glGetUniformLocation(m_background.program, "center"), x_center, y_center);
	m_background.vao.draw();

	glUseProgram(m_tiles.program);
	glUniform1f(glGetUniformLocation(m_tiles.program, "aspect"), aspect);
	glUniform1f(glGetUniformLocation(m_tiles.program, "scale"), scale);
	glUniform2f(glGetUniformLocation(m_tiles.program, "center"), x_center, y_center);
	glUniform1i(glGetUniformLocation(m_tiles.program, "img"), 0);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, m_ui.texture);
	glBindSampler(0, m_sampler);
	m_tiles.vao.draw();

	glUseProgram(m_units.program);
	glUniform1f(glGetUniformLocation(m_units.program, "aspect"), aspect);
	glUniform1f(glGetUniformLocation(m_units.program, "scale"), scale);
	glUniform2f(glGetUniformLocation(m_units.program, "center"), x_center, y_center);
	glUniform1i(glGetUniformLocation(m_units.program, "img"), 0);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, m_ui.texture);
	glBindSampler(0, m_sampler);
	m_units.vao.draw();

	// lights
	glBindFramebuffer(GL_FRAMEBUFFER, m_light_frame);
	glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE); // additive

	glUseProgram(m_light.program);
	glUniform1f(glGetUniformLocation(m_light.program, "aspect"), aspect);
	glUniform1f(glGetUniformLocation(m_light.program, "scale"), scale);
	glUniform2f(glGetUniformLocation(m_light.program, "center"), x_center, y_center);

	point p;
	perception.enumerate_units([&](perception::avatar_t a)
	{
		p = a.position();
		GLfloat outer = 8.0;
		GLfloat inner = 0.0;
		GLfloat elevation = 1.0;

		glUniform1f(glGetUniformLocation(m_light.program, "outerinv"), 1.0f / outer);
		glUniform1f(glGetUniformLocation(m_light.program, "inner"), inner);
		glUniform4f(glGetUniformLocation(m_light.program, "pos"), (GLfloat)p.X, (GLfloat)p.Y, elevation, 1.0f);
		glUniform4f(glGetUniformLocation(m_light.program, "col"), 24.0f, 19.5f, 11.9f, 1.0f);

		std::vector<GLfloat> vertices(points_quad * vertice_depth);
		std::vector<GLuint> indices(index_quad);
		fill_vertex(p, outer * 2, &vertices[0]);
		fill_index(1, &indices[0]);
		m_light.vao.fill(points_quad, { &vertices[0] }, indices);
		m_light.vao.draw();
	});

	// mix
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_scene_size, m_scene_size);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_BLEND);

	glUseProgram(m_scene.program);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, m_scene_texture);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, m_light_texture);
	glUniform1i(glGetUniformLocation(m_scene.program, "img"), 0);
	glUniform1i(glGetUniformLocation(m_scene.program, "light"), 1);
	glUniform3f(glGetUniformLocation(m_scene.program, "rng"), (GLfloat)(std::rand() % 100), (GLfloat)(std::rand() % 100), (GLfloat)(std::rand() % 100));
	m_scene.vao.draw();

	glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// notifications
	glUseProgram(m_notification.program);
	glUniform1f(glGetUniformLocation(m_notification.program, "aspect"), aspect);
	glUniform1f(glGetUniformLocation(m_notification.program, "scale"), scale);
	glUniform2f(glGetUniformLocation(m_notification.program, "center"), x_center, y_center);
	glUniform1i(glGetUniformLocation(m_notification.program, "img"), 0);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, m_popup.texture);
	glBindSampler(0, m_sampler);
	m_notification.vao.draw();

	m_opengl->swap();

#if _DEBUG
	GLenum err = GL_NO_ERROR;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		throw std::runtime_error("OpenGL error");
	}
#endif
}

void renderer::scale(double pan)
{
	m_scale *= 1.0 + (pan / 1000.0);
	m_scale = (std::min)(m_scale, 10.0);
	m_scale = (std::max)(m_scale, 0.01);
}

point renderer::world(const point &screen) const
{
	double fx = 2.0 * screen.X / m_width - 1.0;
	double fy = -2.0 * screen.Y / m_height + 1.0;
	fx = std::round(fx / m_scale);
	fy = std::round(fy / m_scale / m_aspect);
	return { (int)fx, (int)fy };
}