#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT{
    vec3 normal;
} vs_out;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
	gl_Position = view * model * vec4(aPos, 1.0f);
	mat4 nmodel = transpose(inverse(view * model));
	vs_out.normal = normalize(mat3(nmodel) * aNormal);
}