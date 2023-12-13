#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 normal;
out vec3 posWS;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 nmodel;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	posWS = vec3(model * vec4(aPos, 1.0f));
	normal = mat3(nmodel) * aNormal;
	TexCoord = aTexCoord;
}