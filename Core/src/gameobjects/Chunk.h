#pragma once

#include "../graphics/VertexArray.h"
#include "graphics/Camera.h"

#define CHUNK_X 16
#define CHUNK_Y 128
#define CHUNK_Z 16

using namespace CoreGraphics;
using namespace CoreGraphics::Buffers;

namespace CoreGameObjects
{
	enum class BlockType
	{
		UNDEFINED = -2, AIR = -1,

		GRASS = 0,

		DIRT = 2, STONE, SAND
	};

	struct Buffers
	{
		std::vector<glm::tvec4<GLbyte>> positions; // Fourth value indicates the faces: 0(front), 1(back), 2(left), 3(right), 4(top), 5(bottom)
		std::vector<GLushort> uv; // Both coordinates are expected to be packed into one 16-bit unsigned integer
		std::vector<GLbyte> types;
	};

	GLushort PackIntoShort(GLushort val1, GLushort val2);

	class CORE_API Chunk
	{
	private:
		VertexArray* m_VAO;
		signed char m_Blocks[CHUNK_X * CHUNK_Y * CHUNK_Z] = { (signed char)BlockType::UNDEFINED };
		glm::vec3 m_Position;
		glm::mat4 m_Model;

		// 0 - Front, 1 - Back, 2 - Left, 3 - Right
		Chunk* m_ObscuringChunks[4] = { nullptr }; 

		Buffers m_Buffers;

		bool m_ShouldDispose;
		bool m_ShouldRender;
		bool m_IsUploaded;
		bool m_Built;
		bool m_Serialized;
		bool m_Visible;
		unsigned int m_VertexCount;
	private:
		/**
		 * \brief Erases all the vector buffers from memory after they have been uploaded to the GPU
		 */
		void EraseBuffers();
	public:
		/**
		 * \brief Constructs the chunk at a default position of (0,0,0)
		 */
		Chunk();

		/**
		 * \brief Constructs the chunk at a specified position
		 * \param position The position to create a chunk at
		 */
		explicit Chunk(const glm::vec3& position);

		~Chunk();

		/**
		 * \brief Builds the chunk if it has not been built already
		 */
		void Build(bool rebuild = false);

		/**
		 * \brief Initializes the vertex array and uploads the buffers to the GPU
		 */
		void UploadToGPU();

		/**
		 * \brief Orphans the buffer store on the GPU and then updates it entirely
		 */
		void UpdateGPUData();

		/**
		 * \brief Finds all other chunks that might be obscuring this one and stores them as pointers
		 */
		void FindObscuringChunks();

		/**
		 * \brief Checks whether the chunk is visible to the camera. If it is then it should be rendered
		 * \param camera Reference to the scene camera
		 * */
		void CheckVisibility(const Camera& camera);

		VertexArray*& GetVAO() { return m_VAO; }
		BlockType GetBlock(int x, int y, int z) const;
		signed char* GetBlocksPtr() { return m_Blocks;}
		const glm::vec3& GetPos() const { return m_Position; }
		const glm::mat4& GetModel() const { return m_Model; }
		Chunk* GetObscuring(int index) const { return m_ObscuringChunks[index]; }
		bool ShouldDispose() const { return m_ShouldDispose; }
		bool ShouldRender() const { return m_ShouldRender; }
		bool IsUploaded() const { return m_IsUploaded; }
		bool IsBuilt() const { return m_Built; }
		bool Serialized() const { return m_Serialized; }
		bool IsVisible() const { return m_Visible; }
		unsigned int GetVertCount() const { return m_VertexCount; }

		void SetBlock(int x, int y, int z, BlockType type);
		void SetPosition(const glm::vec3& position);
		void SetObscuring(int index, Chunk* chunk) { m_ObscuringChunks[index] = chunk; }
		void SetShouldDispose(bool value) { m_ShouldDispose = value; }
		void SetShouldRender(bool value) { m_ShouldRender = value; }
		void SetSerialized(bool value) { m_Serialized = value; }
		void SetUploaded(bool value) { m_IsUploaded = value; }

		bool operator==(const Chunk& chunk) { return this->GetPos() == chunk.GetPos(); }
	};
}