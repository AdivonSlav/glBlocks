#pragma once

inline void ErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	LOG_ERROR((type == GL_DEBUG_TYPE_ERROR ? "OPENGL ERROR" : "") << " " << type << " " << severity << " " << message)
}

inline void GLFWErrorCallback(int code, const char* description)
{
	LOG_ERROR(code << " " << description)
}

//#define ASSERT(x) if (!(x)) __debugbreak();
//#define GLCall(x) GLClearError();	\
//				x;					\
//				ASSERT(GLLog(#x, __FILE__, __LINE__))
//
//inline void GLClearError()
//{
//	while (glGetError() != GL_NO_ERROR);
//}
//
//inline bool GLLog(const char* function, const char* file, int line)
//{
//	while (GLenum error = glGetError())
//	{
//		LOG_ERROR("(OpenGL Error) " << error << " - " << function << " " << file << ":" << line)
//		return false;
//	}
//
//	return true;
//}
