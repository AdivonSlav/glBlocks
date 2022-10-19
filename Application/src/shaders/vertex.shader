#version 460 core

layout(location = 0) in ivec3 aPos;
layout(location = 1) in uint aType;

flat out ivec3 texCoord;
flat out uint type;

uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uModel;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
	texCoord = aPos;
	type = aType;
}