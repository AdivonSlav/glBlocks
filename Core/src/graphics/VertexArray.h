#pragma once

#include "../CoreAPI.h"

#include "buffers/VertexBuffer.h"

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

		/**
		 * \brief Stores the buffer pointer to the array vector, sets the needed vertex attribute pointers
		 * \param buffer Buffer to be added
		 * \param index The layout index to which the buffer attributes will be bound
		 * \param type The type of data that is held in the buffer
		 */
		void AddBuffer(Buffers::VertexBuffer* buffer, GLint index, GLenum type);

		Buffers::VertexBuffer* GetBuffer(int index) { return m_Buffers.at(index); }

		/**
		 * \brief Binds the VAO
		 */
		void Bind() const;

		/**
		 * \brief Unbinds the VAO
		 */
		void Unbind() const;
	};
}

