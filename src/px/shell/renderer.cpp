// name: renderer.cpp
// type: c++ class implementation
// desc: opengl rendering
// auth: is0urce

#include "stdafx.h"

#include "renderer.h"

#include <px/game.h>
#include <px/ui/canvas.h>
#include <px/shell/font.h>
#include <px/shell/perception.h>
#include <px/shell/glsl.h>
#include <px/vector.h>
#include <px/color.h>
#include <px/string.h>

#include <memory>
#include <stdexcept>
#include <vector>
#include <cmath>

namespace px
{
	namespace shell
	{
		const unsigned int renderer::ui_cell_width = 12;
		const unsigned int renderer::ui_cell_height = 18;

		namespace
		{
			const double move_speed = 3.33;
			const double camera_default = 0.1;
			const unsigned int range_width = game::perc_width;
			const unsigned int range_height = game::perc_height;
			const point range(range_width, range_height);
			const unsigned int range_size = range_width * range_height;
			const unsigned int vertice_depth = 2;
			const unsigned int color_depth = 4;
			const unsigned int texcoord_depth = 2;
			const unsigned int points_quad = 4; // rasterise tiles in 4-points (2-trices)
			const unsigned int index_quad = 6;

			// glyphs & ui
			const char *font_path_ui = "PragmataPro.ttf";
			const char *font_path_notify = "FiraSans-Heavy.otf";
			const char *font_path_unit = "CODE2000.TTF";
			const unsigned int font_size_ui = 18;
			const unsigned int font_size_notify = 32;
			const unsigned int font_size_unit = 64;

			inline vector transform(const vector &v, double cos, double sin)
			{
				return vector(cos * v.X - sin * v.Y, sin * v.X + cos * v.Y);
			}
			inline void fill_vertex(const vector &position, const vector &quad_range, GLfloat *dest)
			{
				(position + vector(-0.5, -0.5) * quad_range).write(dest + 0 * vertice_depth);
				(position + vector(-0.5, 0.5) * quad_range).write(dest + 1 * vertice_depth);
				(position + vector(0.5, 0.5) * quad_range).write(dest + 2 * vertice_depth);
				(position + vector(0.5, -0.5) * quad_range).write(dest + 3 * vertice_depth);
			}
			inline void fill_vertex(const vector &position, const vector &quad_range, double rotation, double scale, GLfloat *dest)
			{
				double cos = std::cos(rotation) * scale;
				double sin = std::sin(rotation) * scale;
				(position + transform(vector(-0.5, -0.5) * quad_range, cos, sin)).write(dest + 0 * vertice_depth);
				(position + transform(vector(-0.5, 0.5) * quad_range, cos, sin)).write(dest + 1 * vertice_depth);
				(position + transform(vector(0.5, 0.5) * quad_range, cos, sin)).write(dest + 2 * vertice_depth);
				(position + transform(vector(0.5, -0.5) * quad_range, cos, sin)).write(dest + 3 * vertice_depth);
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
				if (num == 0) return;
				if (dest.size() == 0) throw std::logic_error("px::renderer::fill_index(int, vector) - vector size = 0, while num != 0");
				fill_index(num, &dest[0]);
			}
		}

		renderer::renderer(renderer::opengl_handle opengl)
			:
			m_aspect(1), m_scale(camera_default),
			m_scene_size(0),

			// drawcalls
			m_background({ vertice_depth, color_depth }, glsl::program("shaders\\ground")),
			m_tiles({ vertice_depth, texcoord_depth, color_depth, color_depth }, glsl::program("shaders\\tiles")),
			m_units({ vertice_depth, vertice_depth, texcoord_depth, color_depth }, glsl::program("shaders\\units")),
			m_light({ vertice_depth }, glsl::program("shaders\\light")),
			m_lightmap({ vertice_depth, color_depth }, glsl::program("shaders\\lightmap")),
			m_lightmap_prev({ vertice_depth, color_depth }, glsl::program("shaders\\lightmap")),
			m_lightdraw({ vertice_depth, texcoord_depth }, glsl::program("shaders\\lightdraw")),
			m_scene({ vertice_depth, texcoord_depth }, glsl::program("shaders\\scene")),
			m_projectiles({ vertice_depth, texcoord_depth, color_depth }, glsl::program("shaders\\projectile")),
			m_notification({ vertice_depth, texcoord_depth, color_depth }, glsl::program("shaders\\notify")),
			m_uiback({ vertice_depth, color_depth }, glsl::program("shaders\\uiback")),
			m_uitext({ vertice_depth, texcoord_depth, color_depth }, glsl::program("shaders\\uitext"))
		{
			if (!opengl) throw std::runtime_error("renderer::renderer(renderer::opengl_handle opengl) opengl is null");

			m_opengl.swap(opengl);

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);

			glClampColor(GL_CLAMP_VERTEX_COLOR, GL_FALSE);
			glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
			glClampColor(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);

			glGenFramebuffers(1, &m_scene_frame);
			glGenFramebuffers(1, &m_light_frame);
			glGenFramebuffers(1, &m_lightmap_frame);

			glGenSamplers(1, &m_sampler);
			glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glSamplerParameteri(m_sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 64);

			glGenTextures(1, &m_ui.texture);
			glGenTextures(1, &m_popup.texture);
			glGenTextures(1, &m_glyph.texture);
			glGenTextures(1, &m_scene_texture);
			glGenTextures(1, &m_light_texture);
			glGenTextures(1, &m_lightmap_texture);
			glGenTextures(1, &m_lightmap_prev_texture);

			m_ui.font.reset(new font(font_path_ui, font_size_ui));
			m_popup.font.reset(new font(font_path_notify, font_size_notify));
			m_glyph.font.reset(new font(font_path_unit, font_size_unit));

			update_textures();

			// setup static lightmap framebuffer
			unsigned int range_max = std::max(range_width, range_height);
			m_lightmap_size = 1;
			while (m_lightmap_size < range_max)
			{
				m_lightmap_size <<= 1;
			}
			glBindTexture(GL_TEXTURE_2D, m_lightmap_texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_lightmap_size, m_lightmap_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glBindTexture(GL_TEXTURE_2D, m_lightmap_prev_texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_lightmap_size, m_lightmap_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

			glBindFramebuffer(GL_FRAMEBUFFER, m_lightmap_frame);
			glBindTexture(GL_TEXTURE_2D, m_lightmap_texture);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightmap_texture, 0);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("lightmap framebuffer not complete");

			// scene vao
			std::vector<GLfloat> vertices(points_quad * vertice_depth);
			std::vector<GLfloat> texture(points_quad * texcoord_depth);
			std::vector<GLuint> indices(index_quad);

			fill_vertex({ 0, 0 }, 2, &vertices[0]);
			fill_texture(0, 0, 1, 1, &texture[0]);
			fill_index(1, &indices[0]);

			m_scene.vao.fill(points_quad, { &vertices, &texture }, indices);

			// ligtdraw vao
			GLfloat draw_area = (GLfloat)range_max / m_lightmap_size;
			//point center(range_width / 2, range_height / 2);
			//fill_vertex({ 0, 0 }, 2, &vertices[0]);
			fill_texture(0, 0, draw_area, draw_area, &texture[0]);
			m_lightdraw.vao.fill(points_quad, { &vertices, &texture }, indices);
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
			glDeleteProgram(m_notification.program);
			glDeleteProgram(m_projectiles.program);
			glDeleteProgram(m_uiback.program);
			glDeleteProgram(m_uitext.program);
			glDeleteProgram(m_lightmap.program);
			glDeleteProgram(m_lightdraw.program);

			glDeleteFramebuffers(1, &m_scene_frame);
			glDeleteFramebuffers(1, &m_light_frame);
			glDeleteFramebuffers(1, &m_lightmap_frame);

			glDeleteTextures(1, &m_ui.texture);
			glDeleteTextures(1, &m_popup.texture);
			glDeleteTextures(1, &m_glyph.texture);
			glDeleteTextures(1, &m_scene_texture);
			glDeleteTextures(1, &m_light_texture);
			glDeleteTextures(1, &m_lightmap_texture);
			glDeleteTextures(1, &m_lightmap_prev_texture);

			glDeleteSamplers(1, &m_sampler);
		}

		void renderer::update_textures()
		{
			if (m_ui.font->updated())
			{
				const font::font_texture &ui_texture = m_ui.font->texture();
				glBindTexture(GL_TEXTURE_2D, m_ui.texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, ui_texture.width, ui_texture.height, 0, GL_RED, GL_UNSIGNED_BYTE, ui_texture.data);
				glEnable(GL_TEXTURE_2D);
				glGenerateMipmap(GL_TEXTURE_2D);
				m_ui.font->updated(false);
			}

			if (m_popup.font->updated())
			{
				const font::font_texture &popup_texture = m_popup.font->texture();
				glBindTexture(GL_TEXTURE_2D, m_popup.texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, popup_texture.width, popup_texture.height, 0, GL_RED, GL_UNSIGNED_BYTE, popup_texture.data);
				glEnable(GL_TEXTURE_2D);
				glGenerateMipmap(GL_TEXTURE_2D);
				m_popup.font->updated(false);
			}

			if (m_glyph.font->updated())
			{
				const font::font_texture &glyph_texture = m_glyph.font->texture();
				glBindTexture(GL_TEXTURE_2D, m_glyph.texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, glyph_texture.width, glyph_texture.height, 0, GL_RED, GL_UNSIGNED_BYTE, glyph_texture.data);
				glEnable(GL_TEXTURE_2D);
				glGenerateMipmap(GL_TEXTURE_2D);
				m_glyph.font->updated(false);
			}
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
			m_background.vao.fill(range_size * points_quad, { &vertices, &colors }, indices);
		}

		void renderer::fill_tiles(const perception_t &perception, font &fnt)
		{
			std::vector<GLfloat> vertices(range_size * points_quad * vertice_depth);
			std::vector<GLfloat> textures(range_size * points_quad * texcoord_depth);
			std::vector<GLfloat> colors(range_size * points_quad * color_depth);
			std::vector<GLfloat> colors_prev(range_size * points_quad * color_depth);
			std::vector<GLuint> indices(range_size * index_quad);

			// vertex attributes
			unsigned int vertex_offset = 0;
			unsigned int color_offset = 0;
			unsigned int texture_offset = 0;
			point(range_width, range_height).enumerate([&](const point &position)
			{
				auto &tile = perception.appearance(position);
				auto g = fnt[tile.image];
				color color_curr = tile.color;
				color color_prev = tile.color;
				color_curr.A = perception.hide(position) ? 0.0 : 1.0;
				color_prev.A = perception.hide_prev(position) ? 0.0 : 1.0;

				fill_vertex(position, { g.width * tile.size, g.height * tile.size }, &vertices[vertex_offset]);
				fill_texture((GLfloat)g.left, (GLfloat)g.bottom, (GLfloat)g.right, (GLfloat)g.top, &textures[texture_offset]);
				fill_color(color_curr, &colors[color_offset]);
				fill_color(color_prev, &colors_prev[color_offset]);

				vertex_offset += vertice_depth * points_quad;
				color_offset += color_depth * points_quad;
				texture_offset += texcoord_depth * points_quad;
			});

			// indices
			fill_index(range_size, indices);

			// bind
			m_tiles.vao.fill(range_size * points_quad, { &vertices, &textures, &colors, &colors_prev }, indices);
		}

		void renderer::fill_units(const perception_t &perception, font &fnt)
		{
			unsigned int unit_num = perception.avatar_count();

			std::vector<GLfloat> vertices(unit_num * points_quad * vertice_depth);
			std::vector<GLfloat> vertices_prev(unit_num * points_quad * vertice_depth);
			std::vector<GLfloat> textures(unit_num * points_quad * texcoord_depth);
			std::vector<GLfloat> colors(unit_num * points_quad * color_depth);
			std::vector<GLuint> indices(unit_num * index_quad);

			// vertex attributes
			unsigned int vertex_offset = 0;
			unsigned int color_offset = 0;
			unsigned int texture_offset = 0; 
			perception.enumerate_avatars([&](const perception::avatar_t &unit)
			{
				auto &appear = unit.appearance();
				auto g = fnt[appear.image];

				fill_vertex(unit.position(), { g.width * appear.size, g.height * appear.size }, &vertices[vertex_offset]);
				fill_vertex(unit.position_previous(), { g.width * appear.size, g.height * appear.size }, &vertices_prev[vertex_offset]);
				fill_texture((GLfloat)g.left, (GLfloat)g.bottom, (GLfloat)g.right, (GLfloat)g.top, &textures[texture_offset]);
				fill_color(unit.appearance().color, &colors[color_offset]);

				vertex_offset += vertice_depth * points_quad;
				color_offset += color_depth * points_quad;
				texture_offset += texcoord_depth * points_quad;
			});

			fill_index(unit_num, indices);

			// bind
			m_units.vao.fill(unit_num * points_quad, { &vertices, &vertices_prev, &textures, &colors }, indices);
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
			std::vector<GLfloat> textures(letters_num * points_quad * texcoord_depth);
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

				pen.move(total_width * -n.size / 2, 0.5); // halfway left and one tile up

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
					fill_color(n.color, &colors[color_offset]);

					pen.X += g.advance * n.size;

					vertex_offset += vertice_depth * points_quad;
					color_offset += color_depth * points_quad;
					texture_offset += texcoord_depth * points_quad;
				});
			});

			fill_index(letters_num, indices);

			// bind
			m_notification.vao.fill(letters_num * points_quad, { &vertices, &textures, &colors }, indices);
		}

		void renderer::fill_projectiles(const perception_t &perception, font &fnt, timespan_t time)
		{
			// calculate number of required letter-squares
			unsigned int count = perception.projectile_count();

			std::vector<GLfloat> vertices(count * points_quad * vertice_depth);
			std::vector<GLfloat> textures(count * points_quad * texcoord_depth);
			std::vector<GLfloat> colors(count * points_quad * color_depth);
			std::vector<GLuint> indices(count * index_quad);

			// vertex attributes
			unsigned int vertex_offset = 0;
			unsigned int color_offset = 0;
			unsigned int texture_offset = 0;
			perception.enumerate_projectiles([&](const projectile &particle)
			{
				auto &appear = particle.appearance();
				auto &glyph = fnt[appear.image];
				auto particle_color = appear.color;
				particle_color.A *= particle.magnitude(time);
				fill_vertex(particle.position(time) - perception.start(), { glyph.width, glyph.height }, particle.rotation(time), particle.scale(time), &vertices[vertex_offset]);
				//fill_vertex(particle.position(time) - perception.start(), { glyph.width, glyph.height }, &vertices[vertex_offset]);
				fill_texture((GLfloat)glyph.left, (GLfloat)glyph.bottom, (GLfloat)glyph.right, (GLfloat)glyph.top, &textures[texture_offset]);
				fill_color(particle_color, &colors[color_offset]);

				vertex_offset += vertice_depth * points_quad;
				color_offset += color_depth * points_quad;
				texture_offset += texcoord_depth * points_quad;
			});

			fill_index(count, indices);

			// bind
			m_projectiles.vao.fill(count * points_quad, { &vertices, &textures, &colors }, indices);
		}

		void renderer::fill_ui(const canvas_t &gui, font &fnt)
		{
			point range = gui.range();
			unsigned int size = range.size();

			std::vector<GLfloat> vertex_front(size * points_quad * vertice_depth); // vertex
			std::vector<GLfloat> vertex_back(size * points_quad * vertice_depth); // vertex
			std::vector<GLfloat> texture(size * points_quad * texcoord_depth);
			std::vector<GLfloat> color_front(size * points_quad * color_depth); // color
			std::vector<GLfloat> color_back(size * points_quad * color_depth); // color
			std::vector<GLuint> indices(size * index_quad);

			// vertex attributes
			unsigned int vertexoffset = 0;
			unsigned int textureoffset = 0;
			unsigned int coloroffset = 0;

			range.enumerate([&](const point &position)
			{
				auto &s = gui.at(position);
				auto &g = fnt[s.code];

				point p(position.X, range.Y - position.Y - 1);

				p.moved(0, 0).multiplied(ui_cell_width, ui_cell_height).write(&vertex_back[vertexoffset + 0 * vertice_depth]);
				p.moved(0, 1).multiplied(ui_cell_width, ui_cell_height).write(&vertex_back[vertexoffset + 1 * vertice_depth]);
				p.moved(1, 1).multiplied(ui_cell_width, ui_cell_height).write(&vertex_back[vertexoffset + 2 * vertice_depth]);
				p.moved(1, 0).multiplied(ui_cell_width, ui_cell_height).write(&vertex_back[vertexoffset + 3 * vertice_depth]);

				int vx = ui_cell_width / 2 - g.pixwidth / 2;
				int vxd = vx + g.pixwidth;
				int vyd = g.pixhorisontal + 1;
				int vy = vyd - g.pixheight;

				p.multiplied(ui_cell_width, ui_cell_height).moved(vx, vy).write(&vertex_front[vertexoffset + 0 * vertice_depth]);
				p.multiplied(ui_cell_width, ui_cell_height).moved(vx, vyd).write(&vertex_front[vertexoffset + 1 * vertice_depth]);
				p.multiplied(ui_cell_width, ui_cell_height).moved(vxd, vyd).write(&vertex_front[vertexoffset + 2 * vertice_depth]);
				p.multiplied(ui_cell_width, ui_cell_height).moved(vxd, vy).write(&vertex_front[vertexoffset + 3 * vertice_depth]);

				fill_color(s.front, &color_front[coloroffset]);
				fill_color(s.back, &color_back[coloroffset]);
				fill_texture((GLfloat)g.left, (GLfloat)g.bottom, (GLfloat)g.right, (GLfloat)g.top, &texture[textureoffset]);

				vertexoffset += vertice_depth * points_quad;
				coloroffset += color_depth * points_quad;
				textureoffset += texcoord_depth * points_quad;
			});

			fill_index(size, indices);

			// bind
			m_uiback.vao.fill(size * points_quad, { &vertex_back, &color_back }, indices);
			m_uitext.vao.fill(size * points_quad, { &vertex_front, &texture, &color_front }, indices);
		}

		void renderer::fill_lightmap(const perception_t &perception)
		{
			std::vector<GLfloat> vertices(range_size * vertice_depth);
			std::vector<GLfloat> colors(range_size * color_depth);
			std::vector<GLfloat> colors_prev(range_size * color_depth);
			std::vector<GLuint> indices(range_size);

			// vertex attributes
			unsigned int vertex_offset = 0;
			unsigned int color_offset = 0;
			point(range_width, range_height).enumerate([&](const point &position)
			{
				color light_color_prev = perception.light_previous(position);

				position.write(&vertices[vertex_offset]);
				perception.light(position).write(&colors[color_offset]);
				perception.light_previous(position).write(&colors_prev[color_offset]);

				vertex_offset += vertice_depth;
				color_offset += color_depth;
			});

			for (unsigned int i = 0; i < range_size; ++i)
			{
				indices[i] = i;
			}

			// bind
			m_lightmap.vao.fill(range_size, { &vertices, &colors }, indices);
			m_lightmap_prev.vao.fill(range_size, { &vertices, &colors_prev }, indices);
		}

		void renderer::draw_geometry(const perception_t &perception, timespan_t timespan)
		{
			// g-eometry
			glBindFramebuffer(GL_FRAMEBUFFER, m_scene_frame);
			glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// background
			glUseProgram(m_background.program);
			glUniform1f(glGetUniformLocation(m_background.program, "aspect"), (GLfloat)m_aspect);
			glUniform1f(glGetUniformLocation(m_background.program, "scale"), (GLfloat)m_scale);
			glUniform2f(glGetUniformLocation(m_background.program, "center"), (GLfloat)m_center.X, (GLfloat)m_center.Y);
			m_background.vao.draw();

			// tiles
			glUseProgram(m_tiles.program);
			glUniform1f(glGetUniformLocation(m_tiles.program, "aspect"), (GLfloat)m_aspect);
			glUniform1f(glGetUniformLocation(m_tiles.program, "scale"), (GLfloat)m_scale);
			glUniform2f(glGetUniformLocation(m_tiles.program, "center"), (GLfloat)m_center.X, (GLfloat)m_center.Y);
			glUniform1f(glGetUniformLocation(m_tiles.program, "phase"), (GLfloat)m_move_phase);
			glUniform1i(glGetUniformLocation(m_tiles.program, "img"), 0);
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, m_glyph.texture);
			glBindSampler(0, m_sampler);
			m_tiles.vao.draw();

			// units
			glUseProgram(m_units.program);
			glUniform1f(glGetUniformLocation(m_units.program, "aspect"), (GLfloat)m_aspect);
			glUniform1f(glGetUniformLocation(m_units.program, "scale"), (GLfloat)m_scale);
			glUniform2f(glGetUniformLocation(m_units.program, "center"), (GLfloat)(perception.range().X / 2), (GLfloat)(perception.range().Y / 2));
			glUniform1f(glGetUniformLocation(m_units.program, "phase"), (GLfloat)m_move_phase);
			glUniform1i(glGetUniformLocation(m_units.program, "img"), 0);
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, m_glyph.texture);
			glBindSampler(0, m_sampler);
			m_units.vao.draw();
		}

		void renderer::draw_light(const perception_t &perception, timespan_t timespan)
		{
			// calculate uniform
			vector lightmap_center = -(vector)perception.movement() * (1.0 - m_move_phase) / range_width * 2;

			glDisable(GL_BLEND);
			glBindFramebuffer(GL_FRAMEBUFFER, m_lightmap_frame);
			glUseProgram(m_lightmap.program);
			glUniform1f(glGetUniformLocation(m_lightmap.program, "tsize"), 2.0f / m_lightmap_size);
			// current
			glBindTexture(GL_TEXTURE_2D, m_lightmap_texture);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightmap_texture, 0);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("lightmap framebuffer not complete");
			glViewport(0, 0, (GLsizei)m_lightmap_size, (GLsizei)m_lightmap_size);
			m_lightmap.vao.draw(GL_POINTS);
			// prev
			glBindTexture(GL_TEXTURE_2D, m_lightmap_prev_texture);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_lightmap_prev_texture, 0);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) throw std::runtime_error("lightmap framebuffer not complete");
			glViewport(0, 0, (GLsizei)m_lightmap_size, (GLsizei)m_lightmap_size);
			m_lightmap_prev.vao.draw(GL_POINTS);

			// lights (non-static)
			glBindFramebuffer(GL_FRAMEBUFFER, m_light_frame);
			glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE); // additive

			// copy static lightmapmap
			glUseProgram(m_lightdraw.program);
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, m_lightmap_texture);
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, m_lightmap_prev_texture);
			glUniform1f(glGetUniformLocation(m_lightdraw.program, "aspect"), (GLfloat)m_aspect);
			glUniform1f(glGetUniformLocation(m_lightdraw.program, "scale"), (GLfloat)m_scale * range_width / 2);
			glUniform2f(glGetUniformLocation(m_lightdraw.program, "center"), GLfloat(lightmap_center.X), GLfloat(lightmap_center.Y));
			glUniform1f(glGetUniformLocation(m_lightdraw.program, "phase"), (GLfloat)m_move_phase); 
			glUniform1i(glGetUniformLocation(m_lightdraw.program, "lmtexture"), 0);
			glUniform1i(glGetUniformLocation(m_lightdraw.program, "lmprevtexture"), 1);
			glBindSampler(0, m_sampler);
			glBindSampler(1, m_sampler);
			m_lightdraw.vao.draw();

			glUseProgram(m_light.program);
			glUniform1f(glGetUniformLocation(m_light.program, "aspect"), (GLfloat)m_aspect);
			glUniform1f(glGetUniformLocation(m_light.program, "scale"), (GLfloat)m_scale);
			glUniform2f(glGetUniformLocation(m_light.program, "center"), (GLfloat)m_center.X, (GLfloat)m_center.Y);

			perception.enumerate_avatars([&](perception::avatar_t a)
			{
				if (a.light().enabled() && !a.light().shadowcasting()) //  && !a.light().shadowcasting()
				{
					vector p = a.position();
					GLfloat outer = 10.0;
					GLfloat inner = 0.0;
					GLfloat elevation = 1.0;
					color light = a.light().color();

					glUniform1f(glGetUniformLocation(m_light.program, "outerinv"), 1.0f / outer);
					glUniform1f(glGetUniformLocation(m_light.program, "inner"), inner);
					glUniform4f(glGetUniformLocation(m_light.program, "pos"), (GLfloat)p.X, (GLfloat)p.Y, elevation, 1.0f);
					glUniform4d(glGetUniformLocation(m_light.program, "col"), light.R, light.G, light.B, light.A);

					std::vector<GLfloat> vertices(points_quad * vertice_depth);
					std::vector<GLuint> indices(index_quad);
					fill_vertex(p, outer * 2, &vertices[0]);
					fill_index(1, &indices[0]);
					m_light.vao.fill(points_quad, { &vertices }, indices);
					m_light.vao.draw();
				}
			});
			perception.enumerate_projectiles([&](const projectile &projectile)
			{
				vector p = projectile.position(m_move_phase) - perception.start();
				GLfloat outer = 10.0;
				GLfloat inner = 0.0;
				GLfloat elevation = 2.0;
				const auto &l = projectile.light();
				auto magnitude = projectile.magnitude(timespan);
				if (l.enabled() && magnitude > 0)
				{
					color light = l.color() * magnitude;
					light.A = 1.0;

					glUniform1f(glGetUniformLocation(m_light.program, "outerinv"), 1.0f / outer);
					glUniform1f(glGetUniformLocation(m_light.program, "inner"), inner);
					glUniform4f(glGetUniformLocation(m_light.program, "pos"), (GLfloat)p.X, (GLfloat)p.Y, elevation, 1.0f);
					glUniform4d(glGetUniformLocation(m_light.program, "col"), light.R, light.G, light.B, light.A);

					std::vector<GLfloat> vertices(points_quad * vertice_depth);
					std::vector<GLuint> indices(index_quad);
					fill_vertex(p, outer * 2, &vertices[0]);
					fill_index(1, &indices[0]);
					m_light.vao.fill(points_quad, { &vertices }, indices);
					m_light.vao.draw();
				}
			});
		}
		void renderer::draw_overlay(const perception_t &perception, const canvas_t &gui, timespan_t timespan)
		{
			glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// particles
			glUseProgram(m_projectiles.program);
			glUniform1f(glGetUniformLocation(m_projectiles.program, "aspect"), (GLfloat)m_aspect);
			glUniform1f(glGetUniformLocation(m_projectiles.program, "scale"), (GLfloat)m_scale);
			glUniform2f(glGetUniformLocation(m_projectiles.program, "center"), (GLfloat)m_center.X, (GLfloat)m_center.Y);
			glUniform1i(glGetUniformLocation(m_projectiles.program, "img"), 0);
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, m_popup.texture);
			glBindSampler(0, m_sampler);
			m_projectiles.vao.draw();

			// notifications
			glUseProgram(m_notification.program);
			glUniform1f(glGetUniformLocation(m_notification.program, "aspect"), (GLfloat)m_aspect);
			glUniform1f(glGetUniformLocation(m_notification.program, "scale"), (GLfloat)m_scale);
			glUniform2f(glGetUniformLocation(m_notification.program, "center"), (GLfloat)m_center.X, (GLfloat)m_center.Y);
			glUniform1f(glGetUniformLocation(m_notification.program, "phase"), (GLfloat)timespan);
			glUniform1i(glGetUniformLocation(m_notification.program, "img"), 0);
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, m_popup.texture);
			glBindSampler(0, m_sampler);
			m_notification.vao.draw();

			// ui
			glUseProgram(m_uiback.program);
			glUniform2f(glGetUniformLocation(m_uiback.program, "scale"), (GLfloat)m_width, (GLfloat)m_height);
			glUniform2f(glGetUniformLocation(m_uiback.program, "offsets"), (GLfloat)(m_width % ui_cell_width / 2), (GLfloat)(m_height % ui_cell_height / 2));
			m_uiback.vao.draw();
			glUseProgram(m_uitext.program);
			glUniform2f(glGetUniformLocation(m_uitext.program, "scale"), (GLfloat)m_width, (GLfloat)m_height);
			glUniform2f(glGetUniformLocation(m_uitext.program, "offsets"), (GLfloat)(m_width % ui_cell_width / 2), (GLfloat)(m_height % ui_cell_height / 2));
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, m_ui.texture);
			m_uitext.vao.draw();
		}
		void renderer::draw(const perception_t &perception, const canvas_t &gui, timespan_t timespan)
		{
			if (perception.range() != range) throw std::logic_error("renderer::draw(..) - perception != range");

			// update clent window bounds
			m_opengl->update(m_width, m_height);
			if (m_width <= 0 || m_height <= 0) return;

			// screen size, time & uniforms
			m_delta = (std::max)(timespan - m_last, 0.0);
			m_last = timespan;
			m_aspect = m_width;
			m_aspect /= m_height;
			m_move_phase = std::max(std::min(timespan * move_speed, 1.0), 0.0);
			m_center = perception.range() / 2 - (vector)perception.movement() * (1.0 - m_move_phase);

			// fill buffers
			fill_bg(perception);
			fill_tiles(perception, *m_glyph.font);
			fill_units(perception, *m_glyph.font);
			fill_projectiles(perception, *m_popup.font, m_move_phase);
			fill_notifications(perception, *m_popup.font);
			fill_ui(gui, *m_ui.font);
			fill_lightmap(perception);

			// update textures with new glyphs
			update_textures();

			// update framebuffer sizes & their textures
			setup_scene();

			// fill geometry and light framebuffers
			draw_geometry(perception, timespan);
			draw_light(perception, m_move_phase);

			// mix geometry and light
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

			// draw projectiles, notifications and ui
			draw_overlay(perception, gui, timespan);

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
			return{ (int)fx, (int)fy };
		}
		point renderer::ui(const point &screen) const
		{
			return point((screen.X - m_width % ui_cell_width / 2) / ui_cell_width, (screen.Y - m_height % ui_cell_height / 2) / ui_cell_height);
		}

		void renderer::size(int &width, int &height)
		{
			m_opengl->update(width, height);
		}
	}
}