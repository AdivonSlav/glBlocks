#pragma once

#include "../CoreAPI.h"

#define BLOCKS_GLSL_VERSION "#version 460"

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
		std::unordered_map<const char*, int> m_Uniforms;

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

		/**
		 * \brief Simple shader caching to prevent OpenGL from querying already known uniforms with glGetUniformLocation
		 * \param uniform Name of the uniform variable in the shader
		 */
		void CacheUniform(const char* uniform);
	public:
		Shader(const char* vertexShader, const char* fragmentShader);
		~Shader();

		/**
		 * \brief Creates and links the shader program
		 */
		void CreateShaderProgram();

		/**
		 * \brief Calls glUniform and sends the provided matrix to the uniform in the shader
		 * \tparam T Type of values in matrix
		 * \param uniform The uniform that will receive the matrix
		 * \param matrix The matrix that is to be sent
		 */
		template<typename T>
		void SetMat4(const char* uniform, const glm::mat4& matrix);

		/*
		template<>
		void SetMat4<float>(const char* uniform, const glm::mat4& matrix);
		*/

		template<typename T>
		void SetVec3(const char* uniform, const glm::vec3& vec);

		/*
		template<>
		void SetVec3<float>(const char* uniform, const glm::vec3& vec);
		*/

		/**
		 * \brief Calls glUniform and sends the provided float to the uniform in the shader
		 * \param uniform The uniform that will receive the float
		 * \param value The value that is to be sent
		 */
		void SetFloat(const char* uniform, GLfloat value);

		/**
		 * \brief Calls glUniform and sends the provided integer to the uniform in the shader
		 * \param uniform The uniform that will receive the integer 
		 * \param value The value that is to be sent
		 */
		void SetInt(const char* uniform, GLint value);

		void Bind();
		void Unbind();

		/**
		 * \brief Returns the shader ID
		 * \return Unsigned integer representing the shader
		 */
		GLuint GetID() const { return m_ProgramID; }
	};
}
