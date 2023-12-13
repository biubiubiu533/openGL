#version 330 core

out vec4 FragColor;

in vec3 TexCoord;

uniform samplerCube skybox;

void main()
{
    vec4 result = texture(skybox, TexCoord);

    FragColor = result;

}