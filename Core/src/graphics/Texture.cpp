#include "Texture.h"
#include <stb/stb_image.h>
#include "../utils/Logger.h"

namespace CoreGraphics
{
	Texture::Texture(GLenum texTarget, const std::string& fileName)
		: m_FileName(fileName), m_TexTarget(texTarget)
	{
	}

	bool Texture::Load()
	{
		// STB, by default, loads images top to bottom. OpenGL expects texture space to be bottom to stop, so we
		// just flip the image on load here.
		stbi_set_flip_vertically_on_load(1);

		int width = 0;
		int height = 0;
		int bpp = 0; // Bits per pixel

		unsigned char* image = stbi_load(m_FileName.c_str(), &width, &height, &bpp, 0);

		if (image == nullptr)
		{
			LOG_ERROR("Can't load texture from path " << m_FileName << " - " << stbi_failure_reason());
			return false;
		}

		LOG_INFO("Loaded texture " << m_FileName);

		glGenTextures(1, &m_TexObj);
		glBindTexture(m_TexTarget, m_TexObj);

		if (m_TexTarget == GL_TEXTURE_3D)
		{
			LOG_ERROR("3D textures are not supported!");
			return false;
		}

		glTexImage2D(m_TexTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		glTexParameterf(m_TexTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(m_TexTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(m_TexTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(m_TexTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(m_TexTarget, 0);
		stbi_image_free(image);

		return true;
	}

	void Texture::Bind(GLenum texUnit) const
	{
		glActiveTexture(texUnit);
		glBindTexture(m_TexTarget, m_TexObj);
	}

	void Texture::Unbind(GLenum texUnit) const
	{
		glBindTexture(m_TexTarget, 0);
	}
}