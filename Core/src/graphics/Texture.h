#pragma once

#include <string>
#include <glad/glad.h>
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
		Texture(GLenum texTarget, const std::string& fileName);
		~Texture() = default;

		bool Load();

		void Bind(GLenum texUnit) const;
		void Unbind(GLenum texUnit) const;
	};
}
