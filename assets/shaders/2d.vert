#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

// outputs to Fragment shader
out vec4 myColor;
out vec4 myTintColor;

uniform mat4 transformation;
uniform vec4 tintColor;

void main()
{
	gl_Position = transformation * vec4(aPos, 1.0);

	myColor = aColor;
	myTintColor = tintColor;
}
