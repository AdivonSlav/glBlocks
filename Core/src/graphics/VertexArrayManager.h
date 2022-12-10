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
		static void CacheVAO(VertexArray* vao);

		static VertexArray* GetVAO();

		static bool IsEmpty();

		static void Cleanup();
	};
}