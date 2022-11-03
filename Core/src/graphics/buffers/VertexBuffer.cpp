#include "VertexBuffer.h"

namespace CoreGraphics::Buffers
{
	VertexBuffer::VertexBuffer(GLuint size, GLint componentCount)
		: m_BufferID(0), m_Size(size), m_ComponentCount(componentCount)
	{
		glGenBuffers(1, &m_BufferID);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}


	void VertexBuffer::BufferData(const void* data, GLenum usage)
	{
		Bind();
		glBufferData(GL_ARRAY_BUFFER, m_Size, data, usage);
		Unbind();
	}

	void VertexBuffer::BufferSubData(GLintptr offset, GLuint size, const void* data)
	{
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
		Unbind();
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
