#version 460 core

layout(location = 0) in ivec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in uint aType;

flat out ivec3 posCoords;
out vec2 texCoords;
flat out uint type;

uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uModel;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
	posCoords = aPos;
	texCoords = aTex;
	type = aType;
}