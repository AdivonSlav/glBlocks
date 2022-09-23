#include "Block.h"

namespace CoreGameObjects
{
	Block::Block(BlockType type, const glm::vec3& position, float scale)
		: m_Type(type), m_Position(position), m_Scale(scale)
	{
        GLfloat vertices[]
        {
            // Front face
            m_Scale *  1.0f, m_Scale *  1.0f, m_Scale *  1.0f,
        	m_Scale * -1.0f, m_Scale *  1.0f, m_Scale *  1.0f,    
            m_Scale * -1.0f, m_Scale * -1.0f, m_Scale *  1.0f,
            m_Scale *  1.0f, m_Scale * -1.0f, m_Scale *  1.0f,

        	// Back face      
            m_Scale *  1.0f, m_Scale *  1.0f, m_Scale * -1.0f,
            m_Scale * -1.0f, m_Scale *  1.0f, m_Scale * -1.0f,
            m_Scale * -1.0f, m_Scale * -1.0f, m_Scale * -1.0f,
            m_Scale *  1.0f, m_Scale * -1.0f, m_Scale * -1.0f,

            // Left face     
            m_Scale * -1.0f, m_Scale *  1.0f, m_Scale *  1.0f, 
            m_Scale * -1.0f, m_Scale *  1.0f, m_Scale * -1.0f, 
            m_Scale * -1.0f, m_Scale * -1.0f, m_Scale * -1.0f, 
            m_Scale * -1.0f, m_Scale * -1.0f, m_Scale *  1.0f,

            // Right face                     
            m_Scale *  1.0f, m_Scale *  1.0f, m_Scale * -1.0f, 
            m_Scale *  1.0f, m_Scale *  1.0f, m_Scale *  1.0f, 
            m_Scale *  1.0f, m_Scale * -1.0f, m_Scale *  1.0f, 
            m_Scale *  1.0f, m_Scale * -1.0f, m_Scale * -1.0f,

            // Top face     
            m_Scale *  1.0f, m_Scale *  1.0f, m_Scale * -1.0f,
            m_Scale * -1.0f, m_Scale *  1.0f, m_Scale * -1.0f,
            m_Scale * -1.0f, m_Scale *  1.0f, m_Scale *  1.0f,
            m_Scale *  1.0f, m_Scale *  1.0f, m_Scale *  1.0f,
                                              m_Scale * 
            // Bottom face   
            m_Scale *  1.0f, m_Scale * -1.0f, m_Scale *  1.0f,
            m_Scale * -1.0f, m_Scale * -1.0f, m_Scale *  1.0f,
            m_Scale * -1.0f, m_Scale * -1.0f, m_Scale * -1.0f,
            m_Scale *  1.0f, m_Scale * -1.0f, m_Scale * -1.0f,
        };

        GLfloat colors[]
        {
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,

            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,

            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,

            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,

            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,

            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
        };

        GLushort indices[]
        {
            //Front face
            0, 1, 2,
            2, 3, 0,

            //Back face
            5, 4, 7,
            7, 6, 5,

            // Left face
		    8,  9,  10,
            10, 11,  8,

            // Right face
            12, 13, 14,
            14, 15, 12,

            // Top face
            16, 17, 18,
            18, 19, 16,

            // Bottom face
            20, 21, 22,
            22, 23, 20
        };

        m_VAO = new VertexArray();
        m_VAO->Bind();

        // Size is calculated manually to be more apparent.
        // 6 corresponds to every face, 4 to vertex count per face and 3 to the component count per vertex.
        // Multiplied by sizeof(GLfloat) it yields the entire size in bytes of the vertices container.
        auto m_PosBuffer = new VertexBuffer(6 * 4 * 3 * sizeof(GLfloat), 3, vertices, GL_STATIC_DRAW);
        m_PosBuffer->Bind();

        auto m_ColorBuffer = new VertexBuffer(6 * 4 * 4 * sizeof(GLfloat), 4, colors, GL_STATIC_DRAW);
        m_ColorBuffer->Bind();

        m_VAO->AddBuffer(m_PosBuffer, 0, GL_FLOAT);
        m_VAO->AddBuffer(m_ColorBuffer, 1, GL_FLOAT);

        m_IBO = new IndexBuffer(6 * 2 * 3 * sizeof(GLushort), 3, indices, GL_STATIC_DRAW);
	}

	Block::~Block()
    {
		delete m_VAO;
        delete m_IBO;
	}
}
