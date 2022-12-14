#version 330 core
in vec4 myColor;
in vec4 myTintColor;
in vec2 myTexCoord;
out vec4 FragColor;

uniform sampler2D myTexture;

void main()
{
	FragColor = myColor * myTintColor * texture(myTexture, myTexCoord);
}
