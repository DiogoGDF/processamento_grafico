#version 400

// variaveis de entrada
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

// variavel de saida
out vec3 vertexColor;

uniform mat4 model;

void main()
{
	vertexColor = color;
	gl_Position = model * vec4(position.x, position.y, position.z, 1.0);
}