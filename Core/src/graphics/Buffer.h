#pragma once

#include <glad/glad.h>
#include "../CoreAPI.h"

namespace CoreGraphics
{
	class Buffer
	{
	private:
		GLuint m_BufferID;
		GLenum m_Type;
	public:
		/**
		 * \brief
		 * \param type Target to which the buffer is bound (e.g. GL_ARRAY_BUFFER)
		 */
		Buffer(GLenum type);
		~Buffer();

		/**
		 * \brief Stores data into the buffer
		 * \param size Size in bytes of the data passed to the buffer
		 * \param data Pointer to the buffer data
		 * \param usage Usage pattern (e.g. GL_STATIC_DRAW)
		 */
		void BufferData(unsigned int size, const void* data, GLenum usage);

		/**
		 * \brief Binds the buffer object
		 */
		void Bind();

		/**
		 * \brief Unbinds the buffer object
		 */
		void Unbind();
	};
}
