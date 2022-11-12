#version 460 core

layout(location = 0) in ivec4 aPos;
layout(location = 1) in uint aTex;
layout(location = 2) in uint aType;

out vec2 texCoords;
out vec3 normal;
out vec3 fragPos;

uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uModel;

uniform int uAtlasSize;

vec2 UnpackTextureCoords()
{
	vec2 coords;

	coords.x = aTex & 0xF;
	coords.y = aTex >> 8 & 0xF;

	return coords;
}

vec2 GetScaledCoords()
{
	vec2 offsets;

	float texColumn = aType % uAtlasSize;
	offsets.x = texColumn / uAtlasSize;

	float texRow = floor(aType / uAtlasSize);
	offsets.y = texRow / uAtlasSize;

	return (UnpackTextureCoords() / uAtlasSize) + offsets;
}

void CalculateNormal()
{
	switch (aPos.w)
	{
	case 0:
		normal = vec3(0.0f, 0.0f, 1.0f);
		break;
	case 1:
		normal = vec3(0.0f, 0.0f, -1.0f);
		break;
	case 2:
		normal = vec3(-1.0f, 0.0f, 0.0f);
		break;
	case 3:
		normal = vec3(1.0f, 0.0f,  0.0f);
		break;
	case 4:
		normal = vec3(0.0f, 1.0f,  0.0f);
		break;
	case 5:
		normal = vec3(0.0f, -1.0f,  0.0f);
		break;
	}
}

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(aPos.x, aPos.y, aPos.z, 1.0);

	texCoords = GetScaledCoords();
	CalculateNormal();

	fragPos = vec3(uModel * vec4(vec3(aPos), 1.0));
}