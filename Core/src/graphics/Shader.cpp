#include "PCH.h"

#include "glad/glad.h"

#include "Shader.h"

namespace CoreGraphics
{
	bool Shader::CheckCompilation(GLuint shaderObject)
	{
		int result;
		glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int logLength;
			glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLength);

			char error[512];
			glGetShaderInfoLog(shaderObject, logLength, &logLength, error);

			int shaderType;
			glGetShaderiv(shaderObject, GL_SHADER_TYPE, &shaderType);

			LOG_ERROR("Failed to compile " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!")
			LOG_ERROR(error)

			return false;
		}

		return true;
	}

	bool Shader::CheckProgramCreation()
	{
		int result;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &result);

		if (result == GL_FALSE)
		{
			int logLength;
			glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &logLength);

			char error[512];
			glGetProgramInfoLog(m_ProgramID, logLength, &logLength, error);

			LOG_ERROR("Failed to link shader program!")
			LOG_ERROR(error)

			return false;
		}

		return true;
	}

	Shader::Shader(const char* vertexShader, const char* fragmentShader)
		: m_ProgramID(0)
	{
		m_Source.vertexSource = ParseShader(vertexShader);
		m_Source.fragmentSource = ParseShader(fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ProgramID);
	}

	std::string Shader::ParseShader(const char* source)
	{
		std::ifstream shaderFile;
		std::stringstream ss;

		shaderFile.open(source);

		if (shaderFile.is_open())
		{
			std::string line;

			while (std::getline(shaderFile, line))
			{
				ss << line << "\n";
			}
		}
		else
		{
			LOG_ERROR("Failed to open shader file! ")
			return "";
		}

		shaderFile.close();

		return ss.str();
	}

	void Shader::CacheUniform(const char* uniform)
	{
		if (!m_Uniforms.contains(uniform))
		{
			m_Uniforms.insert(std::pair(uniform, glGetUniformLocation(m_ProgramID, uniform)));
		}
	}

	GLuint Shader::CompileShader(GLenum type)
	{
		GLuint shaderObject = glCreateShader(type);

		const GLchar* shaderSource = (type == GL_VERTEX_SHADER) ? m_Source.vertexSource.c_str() : m_Source.fragmentSource.c_str();

		if (*shaderSource == '\0')
		{
			LOG_ERROR("Shader parsing failed!")
			glDeleteShader(shaderObject);
			return 0;
		}

		glShaderSource(shaderObject, 1, &shaderSource, nullptr);
		glCompileShader(shaderObject);

		if (!CheckCompilation(shaderObject))
		{
			glDeleteShader(shaderObject);
			return 0;
		}

		return shaderObject;
	}

	void Shader::CreateShaderProgram()
	{
		m_ProgramID = glCreateProgram();
		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER);

		if (vertexShader == 0 || fragmentShader == 0)
		{
			glDeleteProgram(m_ProgramID);
			LOG_ERROR("Ending shader program creation...")
			return;
		}

		glAttachShader(m_ProgramID, vertexShader);
		glAttachShader(m_ProgramID, fragmentShader);
		glLinkProgram(m_ProgramID);

		if (!CheckProgramCreation())
		{
			LOG_ERROR("Ending shader program linking...")
			glDeleteProgram(m_ProgramID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			return;
		}

		glValidateProgram(m_ProgramID);

		glDetachShader(m_ProgramID, vertexShader);
		glDetachShader(m_ProgramID, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	template <typename T>
	void Shader::SetMat4(const char* uniform, const glm::mat4& matrix)
	{

	}

	template <typename T>
	void Shader::SetVec3(const char* uniform, const glm::vec3& vec)
	{

	}

	template <>
	void Shader::SetMat4<float>(const char* uniform, const glm::mat4& matrix)
	{
		CacheUniform(uniform);
		glUniformMatrix4fv(m_Uniforms[uniform], 1, GL_FALSE, glm::value_ptr(matrix));
	}

	template <>
	void Shader::SetVec3<float>(const char* uniform, const glm::vec3& vec)
	{
		CacheUniform(uniform);
		glUniform3fv(m_Uniforms[uniform], 1, glm::value_ptr(vec));
	}

	void Shader::SetFloat(const char* uniform, GLfloat value)
	{
		CacheUniform(uniform);
		glUniform1f(m_Uniforms[uniform], value);
	}

	void Shader::SetInt(const char* uniform, GLint value)
	{
		CacheUniform(uniform);
		glUniform1i(m_Uniforms[uniform], value);
	}

	void Shader::Bind()
	{
		glUseProgram(m_ProgramID);
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}
}
