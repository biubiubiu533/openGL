#version 330 core

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_height1;
    samplerCube skybox;
    sampler2D emissionTex;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambientStrength;
    vec3 diffuseStrength;
    vec3 specularStrength;
};

struct PointLight {
    vec3 position;

    vec3 ambientStrength;
    vec3 diffuseStrength;
    vec3 specularStrength;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutoff;
    float cut;

    vec3 ambientStrength;
    vec3 diffuseStrength;
    vec3 specularStrength;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec3 posWS;
in vec3 normal;
in vec2 TexCoord;

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 posWS);
vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 posWS);

uniform DirLight dirLight;
#define POINT_LIGHTS 4
uniform PointLight pointLights[POINT_LIGHTS];
uniform SpotLight spotLight;

uniform Material material;
uniform float time;
uniform vec3 viewPos;

void main()
{
    vec3 normalWS = normalize(normal); //texture(material.texture_normal1, TexCoord).rgb
    vec3 viewDir = normalize(posWS - viewPos);

    //emission
    vec3 emission = texture(material.emissionTex, TexCoord + vec2(0.0, time)).rgb;

    vec3 result = vec3(0.0);
    result += CalDirLight(dirLight, normalWS, viewDir);
    for (int i = 0; i < POINT_LIGHTS; i++){
        result += CalPointLight(pointLights[i], normalWS, viewDir, posWS);
    }
    result += CalSpotLight(spotLight, normalWS, viewDir, posWS);

    //反射
    vec3 reflectDir = reflect(viewDir, normalWS);
    result += texture(material.skybox, vec3(reflectDir.x, -reflectDir.y, reflectDir.z)).rgb * texture(material.texture_height1, TexCoord).rgb;
    //折射
    float ratio = 1.00 / 1.52;  // air->water
    vec3 refractDir = refract(viewDir, normalWS, ratio);
    vec3 refractCol = texture(material.skybox, vec3(refractDir.x, -refractDir.y, refractDir.z)).rgb;

    FragColor = vec4(result, 1.0);

}

vec3 CalDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);

    //ambient
    vec3 ambient = light.ambientStrength * texture(material.texture_diffuse1, TexCoord).rgb;

    //diffuse
    float lambert = max(0.0, dot(lightDir, normal));
    vec3 diffuse = light.diffuseStrength * lambert * texture(material.texture_diffuse1, TexCoord).rgb;

    //specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = light.specularStrength * pow(max(0.0, dot(reflectDir, viewDir)), material.shininess) * texture(material.texture_specular1, TexCoord).rgb;

    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 CalPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 posWS){
    vec3 lightDir = normalize(light.position - posWS);
    //ambient
    vec3 ambient = light.ambientStrength * texture(material.texture_diffuse1, TexCoord).rgb;

    //diffuse
    float lambert = max(0.0, dot(lightDir, normal));
    vec3 diffuse = light.diffuseStrength * lambert * texture(material.texture_diffuse1, TexCoord).rgb;

    //specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = light.specularStrength * pow(max(0.0, dot(reflectDir, viewDir)), material.shininess) * texture(material.texture_specular1, TexCoord).rgb;

    //attenuation
    float distance    = length(light.position - posWS);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                    light.quadratic * (distance * distance));

    vec3 result = ambient + (diffuse + specular) * attenuation;
    return result;
}

vec3 CalSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 posWS){
    vec3 lightDir = normalize(light.position - posWS);

    //if inside light(theta)
    float lightFragDir = dot(-lightDir, normalize(light.direction));

    //ambient
    vec3 ambient = light.ambientStrength * texture(material.texture_diffuse1, TexCoord).rgb;

    //diffuse
    float lambert = max(0.0, dot(lightDir, normal));
    vec3 diffuse = light.diffuseStrength * lambert * texture(material.texture_diffuse1, TexCoord).rgb;

    //specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = light.specularStrength * pow(max(0.0, dot(reflectDir, viewDir)), material.shininess) * texture(material.texture_specular1, TexCoord).rgb;

    //attenuation
    float distance    = length(light.position - posWS);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                    light.quadratic * (distance * distance));

    vec3 result = ambient + (diffuse + specular) * attenuation;

    float lerp = clamp((light.cutoff - lightFragDir)/(light.cutoff - light.cut), 0.0, 1.0);
    return mix(result, ambient, lerp);
}