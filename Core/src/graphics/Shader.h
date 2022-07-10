#pragma once

#include <string>
#include <glad/glad.h>
#include "../CoreAPI.h"

namespace CoreGraphics
{
	struct ShaderSource
	{
	public:
		std::string vertexSource;
		std::string fragmentSource;
	};

	class CORE_API Shader
	{
	private:
		GLuint m_ProgramID;
		ShaderSource m_Source;

		/**
		 * \brief Checks the compilation status of the shader object
		 * \param shaderObject The shader object from which to retrieve the compile status
		 */
		bool CheckCompilation(GLuint shaderObject);

		/**
		 * \brief Checks the link status of the shader program
		 */
		bool CheckProgramCreation();

		/**
		 * \brief Compiles the shader source code and returns its ID
		 * \param type The shader type to be compiled
		 */
		GLuint CompileShader(GLenum type);

		/**
		 * \brief Parses the shader file into a string
		 * \param source Path to the shader file
		 * \return The shader file parsed as a string
		 */
		std::string ParseShader(const char* source);
	public:
		Shader(const char* vertexShader, const char* fragmentShader);
		~Shader();

		/**
		 * \brief Creates and links the shader program
		 */
		void CreateShaderProgram();

		void Bind();
		void Unbind();
	};
}
