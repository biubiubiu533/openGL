#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform float time;
uniform vec3 viewPos;

void main()
{
    vec4 result = texture(texture1, TexCoord);
    FragColor = result;

}