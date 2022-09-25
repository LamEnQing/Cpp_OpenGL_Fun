#version 330 core
in vec4 myColor;
in vec4 myTintColor;
out vec4 FragColor;

void main()
{
	FragColor = vec4(myColor.x * myTintColor.x, myColor.y * myTintColor.y, myColor.z * myTintColor.z, myColor.w * myTintColor.w);
}
