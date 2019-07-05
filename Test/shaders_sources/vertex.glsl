#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texturePos;


//out vec2 textCord;
out vec3 norm;
out vec3 fragPos;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 temp = model * vec4(position, 1.0f);
	gl_Position = projection * view  * temp;
	//textCord = vec2(texture_pos.x, 1.0f - texture_pos.y);
	norm = mat3(transpose(inverse(model))) * normal;
	fragPos = vec3(temp);
	texCoords = texturePos;
}