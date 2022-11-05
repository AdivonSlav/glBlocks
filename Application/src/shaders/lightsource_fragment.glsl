#version 460 core

flat in uint type;

void main()
{
	if (type == 0)
	{
		gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
	}
	else
	{
		gl_FragColor = vec4(0.81, 0.92, 0.91, 1.0);
	}
}