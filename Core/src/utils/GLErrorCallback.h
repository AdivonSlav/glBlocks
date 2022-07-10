#pragma once

#include <glad/glad.h>
#include "Logger.h"

inline void ErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	LOG_ERROR("(GL CALLBACK) " << (type == GL_DEBUG_TYPE_ERROR ? "OPENGL ERROR" : "") << " " << type << " " << severity << " " << message)
}
