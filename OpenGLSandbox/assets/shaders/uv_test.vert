#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

// outputs to Fragment shader
out vec2 myTexCoord;

//uniform mat4 uTransformation;
uniform vec2 uTexSize;
uniform vec2 uTexOffset;

void main()
{
	gl_Position = /*uTransformation */ vec4(aPos, 1.0, 1.0);

	myTexCoord = aTexCoord * uTexSize + uTexOffset;
}
