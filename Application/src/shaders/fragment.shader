#version 460 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D uSampler;

void main()
{
	fragColor = texture2D(uSampler, texCoord);
}