#include "VertexBuffer.h"

namespace CoreGraphics::Buffers
{
	VertexBuffer::VertexBuffer(GLuint size, GLint componentCount, const void* data, GLenum usage)
		: m_BufferID(0), m_Size(size), m_ComponentCount(componentCount)
	{
		glGenBuffers(1, &m_BufferID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		Unbind();
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
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
