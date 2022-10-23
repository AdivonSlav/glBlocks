#version 460 core

in float isSide;
in vec2 texCoords;
flat in uint type;

out vec4 fragColor;

uniform sampler2D uSampler;
uniform int uAtlasSize;

void main()
{
	vec2 offsets;

	float texColumn = type % uAtlasSize;
	offsets.x = texColumn / uAtlasSize;

	float texRow = floor(type / uAtlasSize);
	offsets.y = texRow / uAtlasSize;

	vec2 scaledTexCoords = (texCoords / uAtlasSize) + offsets;

	gl_FragColor = texture2D(uSampler, scaledTexCoords);
}