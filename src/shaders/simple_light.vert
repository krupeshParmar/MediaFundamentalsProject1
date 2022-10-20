#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normals;
uniform mat4 view, proj, model;
uniform vec3 vertex_colour;
out vec3 colour;
out vec3 normals;
out vec3 frag_position;

void main() { 
	colour = vertex_colour; 
	normals = mat3(transpose(inverse(model))) * vertex_normals; 
	frag_position = vec3(model * vec4(vertex_position, 1.0));
	gl_Position = proj * view * model * vec4(vertex_position, 1.0); 
} 