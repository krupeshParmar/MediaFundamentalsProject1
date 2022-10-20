#version 410
//https://learnopengl.com/Lighting/Basic-Lighting
in vec3 colour;
in vec3 normals;
in vec3 frag_position;
out vec4 frag_colour;
uniform vec3 view_position;

void main() {
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 lightPos = vec3(1.2, 1.0, 2.0);
//ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
//diffuse
	vec3 norm = normalize(normals);
	vec3 lightDir=normalize(lightPos - frag_position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
//specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(view_position - frag_position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * colour;


	frag_colour = vec4(result, 1.0);
}