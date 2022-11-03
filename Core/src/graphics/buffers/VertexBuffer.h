#pragma once

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
		 */
		VertexBuffer(GLuint size, GLint componentCount);
		~VertexBuffer();


		GLuint GetID() const { return m_BufferID; }
		GLuint GetSize() const { return m_Size; }
		GLint GetComponentCount() const { return m_ComponentCount; }

		/**
		 * \brief Calls glBufferData and fills the vertex buffer with the provided data
		 * \param data Pointer to the data being passed to the GPU
		 * \param usage How the memory is intented to be used (e.g. GL_STATIC_DRAW)
		 */
		void BufferData(const void* data, GLenum usage);

		/**
		 * \brief Calls glBufferSubData and replaces all or a part of the vertex buffer's data store
		 * \param offset Offset in bytes into the buffer object's data store where buffering will take place
		 * \param size Size in bytes of the data being replaced
		 * \param data Pointer to the data being passed to the GPU
		 */
		void BufferSubData(GLintptr offset, GLuint size, const void* data);

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
