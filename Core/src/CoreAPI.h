#pragma once

#ifdef BLOCKS_PLATFORM_WINDOWS
	#ifdef BLOCKS_API_EXPORTS
		#define CORE_API __declspec(dllexport)
	#else
		#define CORE_API __declspec(dllimport)
	#endif
#else
	#define CORE_API
#endif