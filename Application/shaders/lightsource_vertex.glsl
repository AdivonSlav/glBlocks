#version 460 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in uint aType;

flat out uint type;

uniform mat4 uProjection;
uniform mat4 uView;

uniform vec3 uWorldPos;
uniform vec3 uCamRight;
uniform vec3 uCamUp;
uniform float uSize;

void main()
{
	//vec3 vertPos = uWorldPos;
	//gl_Position = uProjection * uView  * vec4(vertPos, 1.0f);
	//gl_Position /= gl_Position.w;
	//gl_Position.xy += aPos.xy * vec2(0.05f, 0.05f);

	vec3 vertexPosition = uWorldPos + uCamRight * aPos.x * uSize + uCamUp * aPos.y * uSize;
	gl_Position = uProjection * uView * vec4(vertexPosition, 1.0);

	type = aType;
}