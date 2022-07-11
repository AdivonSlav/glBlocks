#pragma once

#include "../CoreAPI.h"

namespace CoreGraphics
{
	class CORE_API Renderer
	{
	private:

	public:
		Renderer();
		~Renderer() = default;

		void Draw();
	};
}