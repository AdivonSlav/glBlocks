#pragma once

#include <glad/glad.h>
#include "../../CoreAPI.h"

namespace CoreGraphics::Buffers
{
	class CORE_API VertexBuffer
	{
	private:
		GLuint m_BufferID;
		GLuint m_Size;
		GLint m_ComponentCount;
	public:
		/**
		 * \brief Constructs an empty vertex buffer object
		 * \param size The size in bytes of the buffer
		 * \param componentCount Component count per vertex
		 * \param data Pointer to the buffer data
		 * \param usage Hint to OpenGL on how the buffer will be used (e.g. GL_STATIC_DRAW)
		 */
		VertexBuffer(GLuint size, GLint componentCount, const void* data, GLenum usage);
		~VertexBuffer();


		GLuint GetID() const { return m_BufferID; }
		GLint GetSize() const { return m_Size; }
		GLint GetComponentCount() const { return m_ComponentCount; }

		/**
		 * \brief Binds the VertexBuffer object
		 */
		void Bind() const;

		/**
		 * \brief Unbinds the VertexBuffer object
		 */
		void Unbind() const;
	};
}
