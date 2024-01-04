#version 330 core

out vec4 FragColor;

in vec3 posWS;
in vec3 normal;
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
uniform samplerCube skybox;
uniform vec3 viewPos;

void main()
{
    vec3 normalWS = normalize(normal);
    vec3 viewDir = normalize(posWS - viewPos);
    vec4 diffuse = texture(texture_diffuse1, TexCoord);
    vec4 normal = texture(texture_normal1, TexCoord);
    vec4 specular = texture(texture_specular1, TexCoord);

    //折射
    float ratio = 1.00 / 1.52;  // air->water
    vec3 refractDir = refract(viewDir, normalWS, ratio);
    vec4 result = texture(skybox, vec3(refractDir.x, -refractDir.y, refractDir.z));

    FragColor = result;

}