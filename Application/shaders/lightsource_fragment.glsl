#version 460 core

flat in uint type;

out vec4 fragColor;

void main()
{
	if (type == 0)
	{
		fragColor = vec4(1.0, 1.0, 0.0, 1.0);
	}
	else
	{
		fragColor = vec4(0.81, 0.92, 0.91, 1.0);
	}
}