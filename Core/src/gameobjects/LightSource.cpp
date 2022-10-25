
#include "LightSource.h"
#include "World.h"
#include "../utils/Dashboard.h"

namespace CoreGameObjects
{
	LightSource::LightSource()
		: Billboard(), m_Size(1.0f)
	{

	}

	LightSource::LightSource(glm::vec3 position, float scale, LightSourceType type)
		: Billboard(position), m_Size(scale)
	{
		glm::tvec2<GLfloat> positions[4];
		GLbyte types[4];
		glm::tvec3<GLushort> indices[2];

		positions[0] = { 1 * scale, 1 * scale };
		positions[1] = { -1 * scale, 1 * scale };
		positions[2] = { -1 * scale, -1 * scale };
		positions[3] = { 1 * scale, -1 * scale };

		types[0] = (GLbyte)type;
		types[1] = (GLbyte)type;
		types[2] = (GLbyte)type;
		types[3] = (GLbyte)type;

		indices[0] = { 0, 1, 2 };
		indices[1] = { 2, 3, 0 };

		m_VAO.Bind();
		auto vbo = new CoreGraphics::Buffers::VertexBuffer(2 * 4 * sizeof(GLfloat), 2, positions, GL_STATIC_DRAW);
		m_VAO.AddBuffer(vbo, 0, GL_FLOAT);

		auto typeBuffer = new CoreGraphics::Buffers::VertexBuffer(4 * sizeof(GLbyte), 1, types, GL_STATIC_DRAW);
		m_VAO.AddBuffer(typeBuffer, 1, GL_BYTE);

		m_IBO =  new CoreGraphics::Buffers::IndexBuffer(2 * 3 * sizeof(GLushort), 3, indices, GL_STATIC_DRAW);
		m_IndexCount = 2 * 3;
	}

	LightSource::~LightSource()
	{
		delete m_IBO;
	}
}
