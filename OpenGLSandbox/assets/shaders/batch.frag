#version 330 core
in vec4 myColor;
in vec2 myTexCoord;
in float myTexIndex;

out vec4 FragColor;

uniform sampler2D myTextures[2];

void main()
{
	int index = int(myTexIndex);
	FragColor = texture(myTextures[index], myTexCoord);
}
