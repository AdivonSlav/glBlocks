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
		IndexBuffer(GLint size, GLint componentCount, const void* data, GLenum usage);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;
	};
}
