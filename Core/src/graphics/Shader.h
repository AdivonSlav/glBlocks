#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include "../CoreAPI.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	public:
		Shader();
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
		void SetMat4(const char* uniform, glm::mat4& matrix);

		template<>
		void SetMat4<float>(const char* uniform, glm::mat4& matrix);

		void Bind();
		void Unbind();

		GLuint GetID() const { return m_ProgramID; }
	};
}
