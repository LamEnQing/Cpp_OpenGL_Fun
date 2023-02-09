#version 330 core
in vec3 FragPos;
in vec3 myNormal;
in vec2 myTexCoord;

out vec4 FragColor;

uniform vec3 myViewPos;
uniform vec3 myLightPos;
uniform float myLightStrength;
uniform sampler2D myTexture;

void main()
{
	vec3 myLightColor = vec3(1.0f, 1.0f, 1.0f);

	// Ambient
	float ambientStrength = 0.4;
	vec3 ambient = ambientStrength * myLightColor;
	
	// Diffuse
	vec3 norm = normalize(myNormal);
	vec3 lightDir = normalize(myLightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * myLightColor * myLightStrength;
	
	// Specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(myViewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * myLightColor;  

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0) * texture(myTexture, myTexCoord);
}
