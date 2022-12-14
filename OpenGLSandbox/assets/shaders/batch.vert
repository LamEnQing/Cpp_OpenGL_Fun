#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;

// outputs to Fragment shader
out vec4 myColor;
out vec2 myTexCoord;
out float myTexIndex;

//uniform mat4 uTransformation;
//uniform vec2 uTexDimensions;
//uniform vec2 uTexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0, 1.0);

	myColor = aColor;
	myTexCoord = aTexCoord; //* uTexDimensions + uTexCoord;
	myTexIndex = aTexIndex;
}
