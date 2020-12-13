#include "Shader.h"

Shader::Shader()
{
	ID = 0u;
}

void Shader::Create()
{
	ID = glCreateProgram();
}

void Shader::Upload(const char* vertexShader, const char* fragmentShader)
{
	unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertexShader, nullptr);
	glCompileShader(vert);
	CheckError(vert, ShaderType::VertexShader);

	unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragmentShader, nullptr);
	glCompileShader(frag);
	CheckError(vert, ShaderType::FragmentShader);

	glAttachShader(ID, vert);
	glAttachShader(ID, frag);
	glLinkProgram(ID);
	CheckError(vert, ShaderType::ShaderProgram);

	glDeleteShader(vert);
	glDeleteShader(frag);
}

void Shader::Destroy()
{
	glDeleteProgram(ID);
}

void Shader::Use() const
{
	glUseProgram(ID);
}

void Shader::CheckError(unsigned int id, ShaderType type) const
{
	int success;
	char infoLog[1024];
	switch (type)
	{
	case Shader::ShaderType::VertexShader:
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 1024, nullptr, infoLog);
			printf_s("错误：顶点着色器编译失败。错误信息：\n%s\n", infoLog);
		}
		break;
	case Shader::ShaderType::FragmentShader:
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(id, 1024, nullptr, infoLog);
			printf_s("错误：片段着色器编译失败。错误信息：\n%s\n", infoLog);
		}
		break;
	case Shader::ShaderType::ShaderProgram:
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 1024, nullptr, infoLog);
			printf_s("错误：着色器链接失败。错误信息：\n%s\n", infoLog);
		}
		break;
	default:
		break;
	}
}