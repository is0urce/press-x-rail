// name: glsl.h
// type: c++ header
// desc: one header opengl shader compilation functions
// auth: is0urce

#pragma once

#include <fstream>
#include <string>

namespace px
{
	namespace shell
	{
		inline std::string read(const char *path)
		{
			std::string result;
			std::ifstream stream(path, std::ios::in);

			if (!stream.is_open())
			{
				throw std::runtime_error(std::string("can't read file from") + path);
			}

			std::string line = "";
			while (!stream.eof())
			{
				std::getline(stream, line);
				result.append(line + "\n");
			}

			return result;
		}
		inline bool has_suffix(const std::string &str, const std::string &suffix)
		{
			return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
		}

		class glsl
		{
		public:
			static GLuint compile(GLenum type, const std::string &source)
			{
				GLuint shader = glCreateShader(type);
				const char *str = source.c_str();
				glShaderSource(shader, 1, &str, NULL);
				glCompileShader(shader);

				// error handling
				GLint status;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
				if (status == GL_FALSE)
				{
					GLint len;
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
					std::unique_ptr<GLchar[]> log(new GLchar[len + 1]);
					glGetShaderInfoLog(shader, len, NULL, log.get());

					throw std::runtime_error(std::string("shader error:\n") + log.get());
				}

				return shader;
			}

			static GLuint compile(const std::string& name)
			{
				try
				{
					GLenum t = has_suffix(name, ".vert") ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
					return compile(t, read(name.c_str()));
				}
				catch (std::runtime_error& exc)
				{
					throw std::runtime_error(name + "\n" + exc.what());
				}
			}

			static GLuint program(const std::string& vertex, const std::string& fragment)
			{
				GLuint vshader = compile(vertex);
				GLuint fshader = compile(fragment);
				GLuint program = glCreateProgram();
				glAttachShader(program, vshader);
				glAttachShader(program, fshader);
				glLinkProgram(program);
				glDetachShader(program, vshader);
				glDetachShader(program, fshader);
				glDeleteShader(vshader);
				glDeleteShader(fshader);
				return program;
			}
			static GLuint program(const std::string& name)
			{
				std::string v = name + ".vert";
				std::string f = name + ".frag";
				return program(v, f);
			}
		};
	}
}