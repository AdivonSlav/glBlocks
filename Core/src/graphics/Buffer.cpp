#include "Buffer.h"

namespace CoreGraphics
{
	Buffer::Buffer(GLenum type)
		: m_BufferID(0), m_Type(type)
	{
		glGenBuffers(1, &m_BufferID);
	}

	Buffer::~Buffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void Buffer::BufferData(unsigned int size, const void* data, GLenum usage)
	{
		glBufferData(m_Type, size, data, usage);
	}

	void Buffer::Bind()
	{
		glBindBuffer(m_Type, m_Type);
	}

	void Buffer::Unbind()
	{
		glBindBuffer(0, m_BufferID);
	}
}
