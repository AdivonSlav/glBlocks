#version 460 core

flat in ivec3 texCoord;
flat in uint type;

out vec4 fragColor;

uniform sampler2D uSampler;

void main()
{
	fragColor = vec4((type * 10) / 128.0, (type * 10) / 256.0, (type * 10) / 512.0, 1.0);
}