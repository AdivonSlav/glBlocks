#pragma once

//#include "PCH.h"

// Disables the warning relating to exporting an STL class member
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#ifdef BLOCKS_PLATFORM_WINDOWS
	#ifdef BLOCKS_API_EXPORTS
		#define CORE_API __declspec(dllexport)
	#else
		#define CORE_API __declspec(dllimport)
	#endif
#else
	#define CORE_API
#endif

