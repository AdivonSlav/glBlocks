#include "IndexBuffer.h"

namespace CoreGraphics::Buffers
{
	IndexBuffer::IndexBuffer(GLint size, GLint componentCount, const void* data, GLenum usage)
		: m_Size(size), m_ComponentCount(componentCount)
	{
		glGenBuffers(1, &m_BufferID);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
		Unbind();
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(0, m_BufferID);
	}
}
