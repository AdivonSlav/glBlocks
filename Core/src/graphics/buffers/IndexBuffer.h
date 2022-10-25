#pragma once

#include <glad/glad.h>

#include "../../CoreAPI.h"

namespace CoreGraphics::Buffers
{
	class CORE_API IndexBuffer
	{
	private:
		GLuint m_BufferID;
		GLint m_Size;
		GLint m_ComponentCount;
	public:
		/**
		 * \brief Constructs an empty index buffer object
		 * \param size The size in bytes of the buffer
		 * \param componentCount Component count per vertex
		 * \param data Pointer to the buffer data
		 * \param usage Hint to OpenGL on how the buffer will be used (e.g. GL_STATIC_DRAW)
		 */
		IndexBuffer(GLint size, GLint componentCount, const void* data, GLenum usage);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;
	};
}
