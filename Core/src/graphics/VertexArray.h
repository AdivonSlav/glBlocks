#pragma once

#include <vector>
#include <glad/glad.h>

#include "buffers/VertexBuffer.h"
#include "../CoreAPI.h"

namespace CoreGraphics
{
	class CORE_API VertexArray
	{
	private:
		GLuint m_VAO;
		std::vector<Buffers::VertexBuffer*> m_Buffers;

		unsigned int GetTypeSize(GLenum type) const;
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(Buffers::VertexBuffer* buffer, GLint index, GLenum m_Type);

		void Bind() const;
		void Unbind() const;
	};
}

