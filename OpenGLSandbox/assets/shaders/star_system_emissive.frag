#version 330 core
in vec3 FragPos;
in vec3 myNormal;
in vec2 myTexCoord;

uniform float myEmissiveStrength;
out vec4 FragColor;

uniform sampler2D myTexture;

void main()
{
	FragColor = vec4(myEmissiveStrength, myEmissiveStrength, myEmissiveStrength, 1.0) * texture(myTexture, myTexCoord);
}
