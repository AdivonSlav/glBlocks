#version 460 core

in vec3 texCoord;
in float type;

out vec4 fragColor;

uniform sampler2D uSampler;

void main()
{
	fragColor = vec4((type * 10) / 128.0, (type * 10) / 256.0, (type * 10) / 512.0, 1.0);
}