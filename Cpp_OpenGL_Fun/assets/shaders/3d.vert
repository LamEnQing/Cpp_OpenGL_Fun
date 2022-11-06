#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

// outputs to Fragment shader
out vec4 myColor;
out vec4 myTintColor;
out vec2 myTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec4 tintColor;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1.0);

	myColor = aColor;
	myTintColor = tintColor;
	myTexCoord = aTexCoord;
}
