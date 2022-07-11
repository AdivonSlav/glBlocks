#include "Buffer.h"

namespace CoreGraphics
{
	Buffer::Buffer(GLenum target, GLint size, GLint componentCount, const void* data, GLenum usage)
		: m_BufferID(0), m_Target(target), m_Size(size), m_ComponentCount(componentCount)
	{
		glGenBuffers(1, &m_BufferID);
		Bind();
		glBufferData(target, size, data, usage);
		Unbind();
	}

	Buffer::~Buffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}


	void Buffer::Bind() const
	{
		glBindBuffer(m_Target, m_BufferID);
	}

	void Buffer::Unbind() const
	{
		glBindBuffer(0, m_BufferID);
	}
}
