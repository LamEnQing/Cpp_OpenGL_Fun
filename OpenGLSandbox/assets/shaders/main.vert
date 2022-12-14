#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

// outputs to Fragment shader
out vec4 myColor;
out vec4 myTintColor;
out vec2 myTexCoord;

uniform mat4 uTransformation;
uniform vec4 uTintColor;
uniform vec2 uTexDimensions;
uniform vec2 uTexCoord;

void main()
{
	gl_Position = uTransformation * vec4(aPos, 1.0);

	myColor = aColor;
	myTintColor = uTintColor;
	myTexCoord = aTexCoord * uTexDimensions + uTexCoord;
}
