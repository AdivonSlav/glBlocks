#pragma once

namespace CoreUtils
{
	inline void ErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		LOG_ERROR((type == GL_DEBUG_TYPE_ERROR ? "OPENGL ERROR" : "") << " " << type << " " << severity << " " << message)
	}

	inline void GLFWErrorCallback(int code, const char* description)
	{
		LOG_ERROR(code << " " << description)
	}
}
