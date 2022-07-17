#pragma once

#include <glad/glad.h>
#include "../../CoreAPI.h"

namespace CoreGraphics::Buffers
{
	class CORE_API VertexBuffer
	{
	private:
		GLuint m_BufferID;
		GLint m_Size;
		GLint m_ComponentCount;
	public:
		/**
		 * \brief Creates a new VertexBuffer object and fills it with data
		 * \param type Target to which the VertexBuffer is bound (e.g. GL_ARRAY_VertexBuffer)
		 */
		VertexBuffer(GLint size, GLint componentCount, const void* data, GLenum usage);
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
