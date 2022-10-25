#version 460 core

layout(location = 0) in ivec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in uint aType;
layout(location = 3) in vec3 aNormals;

out vec2 texCoords;
flat out uint type;
out vec3 normal;
out vec3 fragPos;

uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uModel;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	texCoords = aTex;
	type = aType;
	normal = aNormals;
	fragPos = vec3(uModel * vec4(aPos, 1.0));
}