#pragma once

#include "VertexArray.h"
#include "../CoreAPI.h"

namespace CoreGraphics
{
	class CORE_API VertexArrayManager
	{
	private:
		static std::queue<VertexArray*> m_Pool;
	public:
		/**
		 * \brief Caches an unused VAO into a queue so it can be reused by a different chunk later on
		 * \param vao Pointer to VAO
		 */
		static void CacheVAO(VertexArray* vao);

		/**
		 * \brief Gets a cached VAO so it can be reused by a chunk
		 * \return Pointer to a cached VAO
		 */
		static VertexArray* GetVAO();

		/**
		 * \brief Returns whether the VAO queue is empty
		 */
		static bool IsEmpty();

		/**
		 * \brief Iterates through the VAO queue, deallocates all VAOs and pops them from the queue
		 */
		static void Cleanup();
	};
}