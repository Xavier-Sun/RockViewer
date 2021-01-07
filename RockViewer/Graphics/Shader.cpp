#include "Shader.h"

void Shader::Upload(const char* vertexShader, const char* fragmentShader)
{
	ID = glCreateProgram();

	unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertexShader, nullptr);
	glCompileShader(vert);
	CheckError(vert, ShaderType::VertexShader);

	unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragmentShader, nullptr);
	glCompileShader(frag);
	CheckError(frag, ShaderType::FragmentShader);

	glAttachShader(ID, vert);
	glAttachShader(ID, frag);
	glLinkProgram(ID);
	CheckError(ID, ShaderType::ShaderProgram);

	glDeleteShader(vert);
	glDeleteShader(frag);
}

void Shader::Use() const
{
	glUseProgram(ID);
}

Shader::Shader()
{
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::CheckError(unsigned int id, ShaderType type) const
{
	int success = 0;
	char infoLog[1024];
	switch (type)
	{
	case Shader::ShaderType::VertexShader:
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 1024, nullptr, infoLog);
			printf_s("ERROR: Failed to compile vertex shader. Message:\n%s\n", infoLog);
		}
		break;
	case Shader::ShaderType::FragmentShader:
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 1024, nullptr, infoLog);
			printf_s("ERROR: Failed to compile fragment shader. Message:\n%s\n", infoLog);
		}
		break;
	case Shader::ShaderType::ShaderProgram:
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 1024, nullptr, infoLog);
			printf_s("ERROR: Failed to link shader program. Message:\n%s\n", infoLog);
		}
		break;
	default:
		break;
	}
}
