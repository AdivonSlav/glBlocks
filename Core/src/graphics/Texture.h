#pragma once

#include "../CoreAPI.h"

namespace CoreGraphics
{
	class CORE_API Texture
	{
	private:
		std::string m_FileName;
		GLenum m_TexTarget;
		GLuint m_TexObj;
	public:
		/**
		 * \brief Constructs a texture object
		 * \param texTarget The target to which the texture will be bound (e.g. GL_TEXTURE_2D)
		 * \param fileName File path to the texture
		 */
		Texture(GLenum texTarget, const std::string& fileName);
		~Texture();

		/**
		 * \brief Loads the texture passed into the object constructor into memory and passes it to a texture object
		 * \return Whether the texture loading was succesfull or not
		 */
		bool Load();

		/**
		 * \brief Binds the current texture object to a texture unit and activates it
		 * \param texUnit The corresponding texture unit (e.g. GL_TEXTURE0) to bind to
		 */
		void Bind(GLenum texUnit) const;

		/**
		 * \brief Unbinds the current texture object
		 */
		void Unbind() const;
	};
}
