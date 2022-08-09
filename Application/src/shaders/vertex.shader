#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

out vec4 color;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	gl_Position = uProjection * uView * vec4(aPos, 1.0);
	color = aColor;
}