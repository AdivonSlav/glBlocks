#pragma once

#ifdef BLOCKS_DEBUG
	#define VERTEX_SHADER "src/shaders/vertex.glsl"
	#define FRAGMENT_SHADER "src/shaders/fragment.glsl"

	#define LIGHT_VERTEX_SHADER "src/shaders/lightsource_vertex.glsl"
	#define LIGHT_FRAGMENT_SHADER "src/shaders/lightsource_fragment.glsl"

	#define TEXTURE_PATH "src/textures/"
#else
	#define VERTEX_SHADER "shaders/vertex.glsl"
	#define FRAGMENT_SHADER "shaders/fragment.glsl"

	#define LIGHT_VERTEX_SHADER "shaders/lightsource_vertex.glsl"
	#define LIGHT_FRAGMENT_SHADER "shaders/lightsource_fragment.glsl"

	#define TEXTURE_PATH "textures/"
#endif