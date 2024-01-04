#version 330 core
layout (triangles) in;   //声明输入的图元类型
layout (triangle_strip, max_vertices = 3) out;   //声明输出的图元类型

in VS_OUT{
    vec2 texCoord;
} gs_in[];

out vec2 TexCoord;
uniform float time;

vec3 GetNormal(){
    vec3 ab = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
    vec3 ac = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
    vec3 normal = normalize(cross(ac, ab));  //注意顺序
    return normal;
}

vec4 explode(vec3 normal, vec4 position){
    float scale = 1.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * scale;
    position += vec4(direction, 0.0);
    return position;
}

void main() {
    gl_Position = explode(GetNormal(), gl_in[0].gl_Position);
    TexCoord = gs_in[0].texCoord;
    EmitVertex();
    gl_Position = explode(GetNormal(), gl_in[1].gl_Position);
    TexCoord = gs_in[1].texCoord;
    EmitVertex();
    gl_Position = explode(GetNormal(), gl_in[2].gl_Position);
    TexCoord = gs_in[2].texCoord;
    EmitVertex();

    EndPrimitive();
}