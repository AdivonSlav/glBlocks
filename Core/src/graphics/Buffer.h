#pragma once

#include <glad/glad.h>
#include "../CoreAPI.h"

namespace CoreGraphics
{
	class CORE_API Buffer
	{
	private:
		GLuint m_BufferID;
		GLenum m_Target;
		GLint m_Size;
		GLint m_ComponentCount;
	public:
		/**
		 * \brief Creates a new buffer object and fills it with data
		 * \param type Target to which the buffer is bound (e.g. GL_ARRAY_BUFFER)
		 */
		Buffer(GLenum target, GLint size, GLint componentCount, const void* data, GLenum usage);
		~Buffer();


		GLuint GetID() const { return m_BufferID; }
		GLint GetSize() const { return m_Size; }
		GLint GetComponentCount() const { return m_ComponentCount; }

		/**
		 * \brief Binds the buffer object
		 */
		void Bind() const;

		/**
		 * \brief Unbinds the buffer object
		 */
		void Unbind() const;
	};
}
