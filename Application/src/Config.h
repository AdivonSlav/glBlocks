#pragma once

#ifdef BLOCKS_DEBUG
	#define VERTEX_SHADER "src/shaders/vertex.shader"
	#define FRAGMENT_SHADER "src/shaders/fragment.shader"

	#define LIGHT_VERTEX_SHADER "src/shaders/lightsource_vertex.shader"
	#define LIGHT_FRAGMENT_SHADER "src/shaders/lightsource_fragment.shader"

	#define TEXTURE_PATH "src/textures/"
#else
	#define VERTEX_SHADER "src/shaders/vertex.shader"
	#define FRAGMENT_SHADER "src/shaders/fragment.shader"

	#define LIGHT_VERTEX_SHADER "src/shaders/lightsource_vertex.shader"
	#define LIGHT_FRAGMENT_SHADER "src/shaders/lightsource_fragment.shader"

	#define TEXTURE_PATH "src/textures/"
#endif