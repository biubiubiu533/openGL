#version 330 core
layout (triangles) in;   //声明输入的图元类型
layout (line_strip, max_vertices = 6) out;   //3 strip * 2 vertices

in VS_OUT{
    vec3 normal;
} gs_in[];

uniform mat4 projection;

vec3 GetNormal(){
    vec3 ab = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
    vec3 ac = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
    vec3 normal = normalize(cross(ac, ab));  //注意顺序
    return normal;
}

void visualize_normal(int index){
    float scale = 0.1;
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * scale);
    EmitVertex();

    EndPrimitive();
}

void main() {
    visualize_normal(0);
    visualize_normal(1);
    visualize_normal(2);
}