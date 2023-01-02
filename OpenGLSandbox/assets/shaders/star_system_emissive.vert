#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// outputs to Fragment shader
out vec3 FragPos;
out vec3 myNormal;
out vec2 myTexCoord;

uniform mat4 uModel;
uniform mat4 uTransformation;
//uniform vec2 uTexDimensions;
//uniform vec2 uTexCoord;

void main()
{
	gl_Position = uTransformation * uModel * vec4(aPos, 1.0f);
	FragPos = vec3(uModel * vec4(aPos, 1.0));

	myNormal = aNormal;
	myTexCoord = aTexCoord; //* uTexDimensions + uTexCoord;
}
