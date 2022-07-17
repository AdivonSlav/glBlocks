#include "VertexArray.h"

namespace CoreGraphics
{
	unsigned int VertexArray::GetTypeSize(GLenum type) const
	{
		switch (type)
		{
		case GL_FLOAT:
			return sizeof(GLfloat);
		case GL_UNSIGNED_INT:
			return sizeof(GLuint);
		case GL_BYTE:
			return sizeof(GLbyte);
		}

		return 0;
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_VAO);
	}

	VertexArray::~VertexArray() 
	{
		for (auto& buffer : m_Buffers)
			delete buffer;
	}

	void VertexArray::AddBuffer(Buffers::VertexBuffer* buffer, GLint index, GLenum type)
	{
		m_Buffers.push_back(buffer);

		Bind();
		buffer->Bind();

		int stride = buffer->GetComponentCount() * GetTypeSize(type);

		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, buffer->GetComponentCount(), type, GL_FALSE, 0, (const void*)0);

		buffer->Unbind();
		Unbind();
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_VAO);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}
