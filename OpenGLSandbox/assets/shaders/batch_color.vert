#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;

// outputs to Fragment shader
out vec4 myColor;

//uniform mat4 uTransformation;
//uniform vec2 uTexDimensions;
//uniform vec2 uTexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0, 1.0);

	myColor = aColor;
}
