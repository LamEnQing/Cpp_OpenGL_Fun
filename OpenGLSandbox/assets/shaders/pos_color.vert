#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

// outputs to Fragment shader
out vec4 myColor;

uniform mat4 uTransformation;
uniform vec3 uTintColor;

void main()
{
	gl_Position = uTransformation * vec4(aPos, 1.0);

	myColor = vec4(uTintColor, 1.0) * aColor;
}
